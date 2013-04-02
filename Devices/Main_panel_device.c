/*
 * Furnace_temp_device.c
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#include "Main_panel_device.h"
#include "Generic_device.h"

// The id of this host on the bus
#define HOST_ID 1

/*
 * Define registers of this device
 */
// This device has 10 registers
__code const unsigned char nr_of_registers = 10;

#define NR_OF_TEMP_SENSORS 2
#define NR_OF_SW_EXTENDERS 1

// Describe the registers of this device
__code const unsigned char register_identification[][REG_IDENTIFICATION_LEN] =
  {
  // Furnace temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Return temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)

      // 2-pin PWM output on extender pins 0 and 1 - gas valves
        { REG_TYPE_PWM2, REG_RW, 3, DONT_CARE, DONT_CARE },
      // Single pin extender outputs

      // Radiator pump
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Floor pump
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Hidraulic Shifter pump
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // HW pump
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Basement floor valve
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Basement radiator valve
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE }, };

/*
 * Onewire specific declarations and defines
 */
#define TEMP1_PINMASK 0x01 // P1_0
#define TEMP2_PINMASK 0x02 // P1_1
bool wait_conv, temp1_conv_initiated, temp2_conv_initiated;

// Buffer to store Temperatures and temp reading timeout
// temperatures are initialized @ 0C. At each unsuccesful reset or read attempt
// the value ONEWIRE_TEMP_FAIL is stored. This value is never the result of a succesful conversion
int temperatures[NR_OF_TEMP_SENSORS];

// Sensors are read in a circular manner. On e cycle completes in time equal to the conversion
// time. This variable holds the id of the sensor to be addressed next during the cycle.
unsigned char next_sensor;

/*
 * Externder switch specific declarations and defines.
 * For extender swich 74HC595, 8-bit parallell or serial out shift registers are used.
 * Extender switches need to be cascaded, meaning that the Q7S pin of registers need to be fed
 * into the DS of the next register, all other control lines should be connected.
 */

#define DS_PIN P1_2
#define SHCP_PIN P1_3
#define NMR_PIN P1_4
#define STCP_PIN P1_5
#define NOE_PIN P1_6

// This value holds the state of extender switches
unsigned char extender_sw_outputs[NR_OF_SW_EXTENDERS];

/*
 * PWM specific defines and variables
 */
#define PWM_STATE_OFF 0
#define PWM_STATE_ON 1
#define PWM_LO_PIN_ID 0
#define PWM_HI_PIN_ID 1

unsigned char pwm_on_time, pwm_off_time, new_pwm_on_time, new_pwm_off_time;
bool is_pwm_low, new_is_pwm_low, load_new_pwm_values, pwm_active;
__bit pwm_state;

bool
set_temp_resolution(unsigned char pinmask, unsigned char resolution)
{
  if (onewire_reset(pinmask))
    {
      onewire_write_byte(CMD_SKIP_ROM, pinmask);
      onewire_write_byte(CMD_WRITE_SCRATCHPAD, pinmask);
      onewire_write_byte(0, pinmask);
      onewire_write_byte(0, pinmask);
      onewire_write_byte(resolution, pinmask);
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

void
scale_DS18B20_result(unsigned char id)
{
  temperatures[id] *= 8;
  temperatures[id] &= 0xFFF0;
  temperatures[id] += 12;
  temperatures[id] -= ow_buf[6];
}

// Calculate which pin is referenced by the pinmask
unsigned char
get_id(unsigned char pinmask)
{
  unsigned char i, id = 0;

  i = pinmask;
  i >>= 1;
  while (i)
    {
      i >>= 1;
      id++;
    }
  return id;
}

void
read_DS18xxx(unsigned char pinmask)
{
  unsigned char i, id;

  id = get_id(pinmask);

  // Reset and read the temperature
  if (onewire_reset(pinmask))
    {
      onewire_write_byte(CMD_SKIP_ROM, pinmask);
      onewire_write_byte(CMD_READ_SCRATCHPAD, pinmask);

      for (i = 0; i < 9; i++)
        {
          ow_buf[i] = onewire_read_byte(pinmask);
        }

      if (ow_buf[8] == calculate_onewire_crc(ow_buf, 8) && ow_buf[7] == 0x10)
        {
          temperatures[id] = ow_buf[0] | (ow_buf[1] << 8);

          // If result needs scaling up then scale it up
          if (register_identification[id][SCALE_POSITION] == SCALE_TEMP)
            scale_DS18B20_result(id);
        }
      else
        {
          temperatures[id] = ONEWIRE_TEMP_FAIL;
        }
    }
}

// Return if conversion initiated succesfully
bool
issue_convert(unsigned char pinmask)
{
  unsigned char id;

  id = get_id(pinmask);

  if (onewire_reset(pinmask))
    {
      onewire_write_byte(CMD_SKIP_ROM, pinmask);
      onewire_write_byte(CMD_CONVERT_T, pinmask);
      return TRUE;
    }
  else
    {
      temperatures[id] = ONEWIRE_TEMP_FAIL;
      return FALSE;
    }
}

// Keep conversions going on for each sensor
void
operate_onewire(void)
{
  if (!wait_conv)
    {
      switch (next_sensor)
        {
      case 1:
        if (temp1_conv_initiated)
          read_DS18xxx(TEMP1_PINMASK);
        temp1_conv_initiated = issue_convert(TEMP1_PINMASK);
        next_sensor = 2;
        break;

      case 2:
        if (temp2_conv_initiated)
          read_DS18xxx(TEMP2_PINMASK);
        temp2_conv_initiated = issue_convert(TEMP2_PINMASK);
        next_sensor = 1;
        break;
        }
      reset_timeout(TEMP_CONV_TIMER);
      wait_conv = TRUE;
    }
  else
    {
      wait_conv = !timeout_occured(TEMP_CONV_TIMER, DS18x20_CONV_TIME / 2);
    }
}

// Reset the extender switches
void
reset_extender_switches(void)
{
  NOE_PIN = 0;
  DS_PIN = 0;
  SHCP_PIN = 0;
  NMR_PIN = 0;

  STCP_PIN = 0;
  STCP_PIN = 1;
  STCP_PIN = 0;

  NMR_PIN = 1;
  NOE_PIN = 1;
}

// Set the output to the values passed
void
write_extender_switches(void)
{
  unsigned char i, j, mask;

  j = NR_OF_SW_EXTENDERS;

  // Loop through all the extender switch values
  while (j--)
    {
      i = extender_sw_outputs[j];
      mask = 0x80;
      while (mask)
        {
          DS_PIN = (i & mask) > 0;
          SHCP_PIN = 1;
          SHCP_PIN = 0;

          mask >>= 1;
        }
    }
  STCP_PIN = 1;
  STCP_PIN = 0;
}

// Return the value of the extender register referenced
// 0 is the first and 7 is the last in an 8 SW register
get_extender_switch_value(unsigned char reg_nr)
{
  unsigned char mask;

  mask = 0x01 << (reg_nr % 8);

  return (extender_sw_outputs[reg_nr / 8] & mask) > 0;
}

// Set the extender switch value
// 0 is the first and 7 is the last in an 8 SW register
void
set_extender_switch_value(unsigned char reg_nr, unsigned char value)
{
  unsigned char mask;

  mask = 0x01 << (reg_nr % 8);

  if (value)
    extender_sw_outputs[reg_nr / 8] |= mask;
  else
    extender_sw_outputs[reg_nr / 8] &= ~mask;

  write_extender_switches();
}

// Set the new PWM vaues and determine PWM state based on it
void
set_new_pwm_values(void)
{
  // Load the new PWM values
  pwm_on_time = new_pwm_on_time;
  pwm_off_time = new_pwm_off_time;
  is_pwm_low = new_is_pwm_low;
  load_new_pwm_values = FALSE;

  // Set if PWM is active for code readibility
  pwm_active = pwm_on_time > 0;
}

// Activate the PWM output values on the extender outputs and reset PWM timer
void
activate_pwm_output(__bit _pwm_state)
{
  pwm_state = _pwm_state;
  if (is_pwm_low)
    {
      set_extender_switch_value(PWM_HI_PIN_ID, 0);
      set_extender_switch_value(PWM_LO_PIN_ID, _pwm_state);
    }
  else
    {
      set_extender_switch_value(PWM_LO_PIN_ID, 1);
      set_extender_switch_value(PWM_HI_PIN_ID, _pwm_state);
    }
  reset_timeout(PWM1_TIMER);
}

// Must be called periodically to take care of PWM outputs
void
operate_PWM()
{
  // If the PWM is operational
  if (pwm_active)
    {
      if (pwm_state == PWM_STATE_ON) // If State is on
        {
          // Times are given in 10th seconds so we need to multiply by 100
          if (timeout_occured(PWM1_TIMER, pwm_on_time * 100))
            activate_pwm_output(PWM_STATE_OFF);
        }
      else // If State is off
        {
          // Times are given in 10th seconds so we need to multiply by 100
          if (timeout_occured(PWM1_TIMER, pwm_off_time * 100))
            {
              // Check if new values need to be set and set them if needed
              if (load_new_pwm_values)
                set_new_pwm_values();

              // If PWM is active based on effective timing values then change state
              if (pwm_active)
                activate_pwm_output(PWM_STATE_ON);
            }
        }
    }
  // In PWM is not active then see if new PWM values need to be loaded
  else if (load_new_pwm_values)
    {
      set_new_pwm_values();

      // If the new values set the state to active then start the PWM
      if (pwm_active)
        activate_pwm_output(PWM_STATE_ON);
    }
}

/*
 * To send a PING:
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,07,01,01,36,05,37,cf}
 *
 * To identify Register 4 (expected response as per the above identification data):
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,08,01,01,36,02,04,2c,d8}
 *
 * To identify Register 1 (expected response as per the above identification data):
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,08,01,01,36,02,01,99,32}
 *
 * To Read Register 1
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,08,01,01,36,01,01,8f,66}
 *
 * To Write Register 3 (PWM), with low pin 2-2 sec
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,0b,01,01,35,00,03,14,14,00,04,d8}
 *
 */
void
operate_device(void)
{

  // Create messaging variables
  unsigned char response_opcode = RESPONSE_UNDEFINED, p;

  // The main loop of the device
  while (TRUE)
    {
      // Operate main device functions
      operate_onewire();
      operate_PWM();

      // Take care of messaging
      if (get_device_message() && !process_generic_messages())
        {
          switch (message_buffer.content[OPCODE])
            {
          case SET_REGISTER:
            // p holds register to write
            p = message_buffer.content[PARAMETER_START];
            // Registers below 3 are read only temp registers and we have 1 PWM and 6 SW registers
            // Register 3 is a 2-extender pin PWM register
            if (p == 3)
              {
                // Setup new pwm parameters
                new_pwm_on_time = message_buffer.content[PARAMETER_START + 1];
                new_pwm_off_time = message_buffer.content[PARAMETER_START + 2];
                new_is_pwm_low = (message_buffer.content[PARAMETER_START + 3]
                    == 0);

                // Set load flag for operate_PWM
                load_new_pwm_values = TRUE;

                response_opcode = COMMAND_SUCCESS;
              }
            // Registers 4 to 9 are SW registers
            else if (p > 3 && p < 10)
              {
                // Map register number to extender switch number (4 maps to 2; 9 maps to 7)
                p -= 2;

                set_extender_switch_value(p,
                    message_buffer.content[PARAMETER_START + 1]);
                response_opcode = COMMAND_SUCCESS;
              }
            else
              {
                response_opcode = COMMAND_FAIL;
              }
            break;
          case READ_REGISTER:
            // p holds register to read
            p = message_buffer.content[PARAMETER_START];
            // Branch based on register number
            if (p < 3)
              {
                message_buffer.content[PARAMETER_START] = temperatures[p - 1]
                    & 0x00ff;
                message_buffer.content[PARAMETER_START + 1] = (temperatures[p
                    - 1] >> 8) & 0x00ff;
                message_buffer.index = PARAMETER_START + 1;

                response_opcode = COMMAND_SUCCESS;
              }
            else if (p == 3)
              {
                message_buffer.content[PARAMETER_START + 1] = new_pwm_on_time;
                message_buffer.content[PARAMETER_START + 2] = new_pwm_off_time;
                message_buffer.content[PARAMETER_START + 3] = new_is_pwm_low;

                response_opcode = COMMAND_SUCCESS;
              }
            else if (p > 3 && p < 10)
              {
                // Map register number to extender switch number (4 maps to 2; 9 maps to 7)
                p -= 2;

                message_buffer.content[PARAMETER_START] =
                    get_extender_switch_value(p);
                message_buffer.index = PARAMETER_START;
                response_opcode = COMMAND_SUCCESS;
              }
            else
              {
                response_opcode = COMMAND_FAIL;
              }
            break;
          default:
            response_opcode = COMMAND_FAIL;
            break;
            }
          send_response(response_opcode);
        }
    }
}

void
device_specific_init(void)
{
  unsigned char i;

  i = NR_OF_TEMP_SENSORS;
  while (i--)
    temperatures[i - 1] = 0;

  // Set initial resolutions to 12 bit
  set_temp_resolution(TEMP_RESOLUTION_12BIT, TEMP1_PINMASK);
  set_temp_resolution(TEMP_RESOLUTION_12BIT, TEMP2_PINMASK);

  wait_conv = FALSE;

  temp1_conv_initiated = temp2_conv_initiated = FALSE;
  next_sensor = 1;

  // Reset conversion timers and distribute conversion across the 3 sensors
  reset_timeout(TEMP_CONV_TIMER);

  // Reset the outputs of the extenders
  i = NR_OF_SW_EXTENDERS;
  while (i--)
    extender_sw_outputs[i] = 0;

  reset_extender_switches();

  // Reset PWM
  pwm_on_time = pwm_off_time = new_pwm_on_time = new_pwm_off_time = 0;
  is_pwm_low = TRUE;
  load_new_pwm_values = FALSE;
  pwm_state = PWM_STATE_OFF;
  pwm_active = FALSE;

}

void
main(void)
{
// Enable interrupts and initialize timer
  EA = 1;
  init_timer();

// Set 4800 baud
  init_device_comm(HOST_ID, COMM_SPEED_4800_H);

// onewire_test();
  device_specific_init();

  operate_device();

}
