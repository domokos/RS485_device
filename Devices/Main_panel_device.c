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
#define NR_OF_OW_BUSES 2
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
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE } };

/*
 * Onewire specific declarations and defines
 */
// Map registers to onewire buses Register1 is on P1_0, Register2 is on P1_1
__code const unsigned char register_pinmask_map[2] =
  { 0x01, 0x02 };

// Store 64 bit rom values of registers/devices
__code const unsigned char register_rom_map[][8] =
  {
  // First byte is zero, only one device on bus
        { 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 },
      // First byte is zero, only one device on bus
        { 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 } };

bool conv_complete, bus0_conv_initiated, bus1_conv_initiated;

// Buffer to store Temperatures and temp reading timeout
// temperatures are initialized @ 0C. At each unsuccesful reset or read attempt
// the value ONEWIRE_TEMP_FAIL is stored. This value is never the result of a succesful conversion
int temperatures[NR_OF_TEMP_SENSORS];

// Sensors are read in a circular manner. On e cycle completes in time equal to the conversion
// time. This variable holds the id of the sensor to be addressed next during the cycle.
unsigned char bus_to_address;

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
#define PWM_OFF 0
#define PWM_LO_OFF 1
#define PWM_MID 2
#define PWM_HI_ON 3
#define PWM_ON 4
#define PWM_REQ_HI 5
#define PWM_REQ_LO 6
#define PWM_LO_PIN_ID 0
#define PWM_HI_PIN_ID 1

// Variables holding PWM times and state
unsigned char pwm_on_time, pwm_off_time, new_pwm_on_time, new_pwm_off_time,
    pwm_state, requested_PWM_state;

// Variables holding PWM modification related flags
bool is_pwm_low, new_is_pwm_low, load_new_pwm_values;

/*
 * Functions of the device
 *
 * ONEWIRE devices specific section
 */

bool
set_temp_resolution(unsigned char register_id, unsigned char resolution)
{
  unsigned char pinmask = register_pinmask_map[register_id];

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
scale_DS18B20_result(unsigned char register_id)
{
  temperatures[register_id] *= 8;
  temperatures[register_id] &= 0xFFF0;
  temperatures[register_id] += 12;
  temperatures[register_id] -= ow_buf[6];
}

void
send_onewire_rom_commands(unsigned char register_id)
{
  unsigned char i, pinmask = register_pinmask_map[register_id];

  // If no ROM is specified for this device (there is only one device on this bus)
  // issue a "SKIP ROM" otherwise issue a "MATCH ROM" followed by the ROM code
  if (register_rom_map[register_id][0] == 0)
    onewire_write_byte(CMD_SKIP_ROM, pinmask);
  else
    {
      i = 0;
      // Issue "MATCH ROM"
      onewire_write_byte(CMD_MATCH_ROM, pinmask);

      // Write the 64 bit ROM code
      do
        onewire_write_byte(register_rom_map[register_id][i], pinmask);
      while (i++ < 7);
    }
}

// Read a DS18xx sensor
void
read_DS18xxx(unsigned char register_id)
{
  unsigned char i, pinmask = register_pinmask_map[register_id];

  // Reset and read the temperature
  if (onewire_reset(pinmask))
    {
      send_onewire_rom_commands(register_id);
      onewire_write_byte(CMD_READ_SCRATCHPAD, pinmask);

      for (i = 0; i < 9; i++)
        {
          ow_buf[i] = onewire_read_byte(pinmask);
        }

      if (ow_buf[8] == calculate_onewire_crc(ow_buf, 8) && ow_buf[7] == 0x10)
        {
          temperatures[register_id] = ow_buf[0] | (ow_buf[1] << 8);

          // If result needs scaling up then scale it up
          if (register_identification[register_id][SCALE_POSITION] == SCALE_TEMP)
            scale_DS18B20_result(register_id);
        }
      else
        {
          temperatures[register_id] = ONEWIRE_TEMP_FAIL;
        }
    }
}

// Return if conversion command is sent succesfully
// It takes a reference to a specific device but issues convert on the entire bus
bool
issue_convert_on_bus(unsigned char register_id)
{
  unsigned char pinmask = register_pinmask_map[register_id];

  if (onewire_reset(pinmask))
    {
      onewire_write_byte(CMD_SKIP_ROM, pinmask);
      onewire_write_byte(CMD_CONVERT_T, pinmask);
      return TRUE;
    }
  return FALSE;
}

// Keep conversions going on for each sensor on each onewire bus
void
operate_onewire_temp_measurement(void)
{
  if (conv_complete)
    {
      switch (bus_to_address)
        {
      case 0:
        if (bus0_conv_initiated)
          read_DS18xxx(0);
        bus0_conv_initiated = issue_convert_on_bus(0);
        bus_to_address = 1;
        break;

      case 1:
        if (bus1_conv_initiated)
          read_DS18xxx(1);
        bus1_conv_initiated = issue_convert_on_bus(1);
        bus_to_address = 0;
        break;
        }

      // Reset the conversion timer and set the complete flag so we
      // can wait for conversion time expiry on the next bus
      reset_timeout(TEMP_CONV_TIMER);
      conv_complete = FALSE;
    }
  else
    {
      conv_complete = timeout_occured(TEMP_CONV_TIMER,
          DS18x20_CONV_TIME / NR_OF_OW_BUSES);
    }
}

/*
 * EXTENDER Switches specific function section
 *
 */

// Reset the extender switches
void
reset_extender_switches(void)
{
  NOE_PIN = 1;
  DS_PIN = 0;
  SHCP_PIN = 0;
  NMR_PIN = 0;

  STCP_PIN = 0;
  STCP_PIN = 1;
  STCP_PIN = 0;

  NMR_PIN = 1;
  NOE_PIN = 0;
}

// Set the output to the values passed
void
write_extender_switches(void)
{
  unsigned char i, j, mask;

  j = NR_OF_SW_EXTENDERS;

  // Reset the shift registers
  NMR_PIN = 0;
  NMR_PIN = 1;

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
__bit
get_extender_switch(unsigned char reg_nr)
{
  unsigned char mask;

  mask = 0x01 << (reg_nr % 8);

  return (extender_sw_outputs[reg_nr / 8] & mask) > 0;
}

// Set the extender buffer value
// 0 is the first and 7 is the last in an 8 SW register
void
set_extender_switch_buffer(unsigned char reg_nr, __bit value)
{
  unsigned char mask;

  mask = 0x01 << (reg_nr % 8);

  if (value)
    extender_sw_outputs[reg_nr / 8] |= mask;
  else
    extender_sw_outputs[reg_nr / 8] &= ~mask;
}

// Set the new PWM vaues and return if state change logic needs to be reversed
void
set_new_pwm_values(void)
{

  // Load the new PWM values
  pwm_on_time = new_pwm_on_time;
  pwm_off_time = new_pwm_off_time;
  is_pwm_low = new_is_pwm_low;

  load_new_pwm_values = FALSE;

  if (pwm_on_time == 0 && pwm_off_time == 1)
    requested_PWM_state = PWM_OFF;
  else if (pwm_on_time == 0 && pwm_off_time == 1)
    requested_PWM_state = PWM_ON;
  else if (is_pwm_low)
    requested_PWM_state = PWM_REQ_LO;
  else
    requested_PWM_state = PWM_REQ_HI;
}

// Activate the PWM output values on the extender outputs and reset PWM timer
void
activate_pwm_state(unsigned char next_pwm_state)
{
  pwm_state = next_pwm_state;
  switch (pwm_state)
    {
  case PWM_OFF:
  case PWM_LO_OFF:
    set_extender_switch_buffer(PWM_LO_PIN_ID, 0);
    set_extender_switch_buffer(PWM_HI_PIN_ID, 0);
    break;
  case PWM_MID:
    set_extender_switch_buffer(PWM_LO_PIN_ID, 1);
    set_extender_switch_buffer(PWM_HI_PIN_ID, 0);
    break;
  case PWM_HI_ON:
  case PWM_ON:
    set_extender_switch_buffer(PWM_LO_PIN_ID, 1);
    set_extender_switch_buffer(PWM_HI_PIN_ID, 1);
    break;
    }
  write_extender_switches();
  reset_timeout(PWM1_TIMER);
}

unsigned char
evaluate_state_change_rule1(void)
{
  if (requested_PWM_state == PWM_OFF)
    return PWM_OFF;
  else
    return PWM_MID;
}

unsigned char
evaluate_state_change_rule2(void)
{
  if (requested_PWM_state == PWM_OFF)
    return PWM_OFF;
  else if (requested_PWM_state == PWM_REQ_LO)
    return PWM_LO_OFF;
  else if (requested_PWM_state == PWM_REQ_HI)
    return PWM_HI_ON;
  else
    // If requested state is PWM_ON
    return PWM_ON;
}

unsigned char
evaluate_state_change_rule3(void)
{
  if (requested_PWM_state == PWM_ON)
    return PWM_ON;
  else
    return PWM_MID;
}

// Must be called periodically to take care of PWM outputs
void
operate_PWM(void)
{
  unsigned char next_state, wait_time;
  next_state = pwm_state;

  if (pwm_state == PWM_OFF || pwm_state == PWM_ON)
    {
      if (load_new_pwm_values)
        {
          set_new_pwm_values();
          if (pwm_state == PWM_OFF)
            next_state = evaluate_state_change_rule1();
          else
            next_state = evaluate_state_change_rule3();
        }
    }
  else
    {
      if (pwm_state == PWM_LO_OFF || (pwm_state == PWM_MID && is_pwm_low))
        wait_time = pwm_off_time;
      else
        wait_time = pwm_on_time;

      if (timeout_occured(PWM1_TIMER, wait_time * 10))
        {
          if (!load_new_pwm_values)
            {
              if (requested_PWM_state == PWM_OFF)
                next_state = PWM_OFF;
              else if (requested_PWM_state == PWM_ON)
                next_state = PWM_ON;
              else if (pwm_state == PWM_MID && is_pwm_low)
                next_state = PWM_LO_OFF;
              else if (pwm_state == PWM_MID)
                next_state = PWM_HI_ON;
              else
                // if (pwm_state == PWM_HI_ON || pwm_state == PWM_LO_OFF)
                next_state = PWM_MID;
            }
          else
            {
              set_new_pwm_values();
              if (pwm_state == PWM_LO_OFF)
                next_state = evaluate_state_change_rule1();
              else if (pwm_state == PWM_MID)
                next_state = evaluate_state_change_rule2();
              else
                // if (pwm_state == PWM_HI_ON)
                next_state = evaluate_state_change_rule3();
            }
        }
    }

  if (next_state != pwm_state)
    activate_pwm_state(next_state);
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
      operate_onewire_temp_measurement();
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

                set_extender_switch_buffer(p,
                    message_buffer.content[PARAMETER_START + 1] == 1);
                write_extender_switches();

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

                message_buffer.content[PARAMETER_START] = get_extender_switch(
                    p);
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
  set_temp_resolution(0, TEMP_RESOLUTION_12BIT);
  set_temp_resolution(1, TEMP_RESOLUTION_12BIT);

  // We need to start a new conversion so it is complete on init
  conv_complete = TRUE;

  bus0_conv_initiated = bus1_conv_initiated = FALSE;
  bus_to_address = 0;

  // Reset conversion timers and distribute conversion across the 3 sensors
  reset_timeout(TEMP_CONV_TIMER);

  // Reset the outputs of the extenders
  i = NR_OF_SW_EXTENDERS;
  while (i--)
    extender_sw_outputs[i] = 0;

  reset_extender_switches();

  // Reset PWM
  pwm_on_time = pwm_off_time = new_pwm_on_time = new_pwm_off_time = 0;
  is_pwm_low = new_is_pwm_low = TRUE;
  load_new_pwm_values = FALSE;
  pwm_state = requested_PWM_state = PWM_OFF;

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
