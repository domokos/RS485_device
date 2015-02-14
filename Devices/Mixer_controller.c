/*
 * Mixer_controller.c
 *
 *  Created on: Jan 30, 2015
 *      Author: dmolnar
 */

#include "Generic_device.h"
#include "Mixer_controller.h"

/*
 * Define registers of this device
 */
// This device has 10 registers
__code const unsigned char nr_of_registers = 10;

#define NR_OF_TEMP_SENSORS 5

// Describe the registers of this device
__code const unsigned char register_identification[][REG_IDENTIFICATION_LEN] =
  {
      // Forward floor temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Incoming floor temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Return floor temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Upper buffer sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Lower buffer sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)

      // CW valve output - Contact CW
        { REG_TYPE_PULSE, REG_WO, 2, DONT_CARE, DONT_CARE },
      // CCW valve output - Contact CCW
        { REG_TYPE_PULSE, REG_WO, 2, DONT_CARE, DONT_CARE },
      // Spare switch A - Contact SPA
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Spare switch B - Contact SPB
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // GPIO switch 2 - Contact 1 on the lowpower part of the pcb
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE } };


/*
 * Onewire specific declarations and defines
 */

// Map registers to onewire buses all registers are P3_3
__code const unsigned char register_pinmask_map[NR_OF_TEMP_SENSORS] =
  {0x01, 0x02, 0x02, 0x04, 0x04};

// Store 64 bit rom values of registers/devices
__code const unsigned char register_rom_map[][8] =
  {
  // If the first byte is zero, then there is only one device on bus
      // Forward floor temp sensor
        { 0x10, 0xc7, 0xa2, 0x23, 0x01, 0x08, 0x00, 0x74 },
      // Incoming floor temp sensor
        { 0x10, 0x47, 0xbf, 0x24, 0x01, 0x08, 0x00, 0x88 },
      // Return floor temp sensor
        { 0x28, 0x91, 0x2d, 0x50, 0x01, 0x00, 0x00, 0xff },
      // Upper buffer sensor
        { 0x28, 0x5a, 0xe6, 0x48, 0x01, 0x00, 0x00, 0xee },
      // Lower buffer sensor
        { 0x28, 0x5a, 0xe6, 0x48, 0x01, 0x00, 0x00, 0xee }};

bool conv_complete;

// Bitfield to store register conversion initiation information
unsigned char register_conv_initiated;

// Buffer to store Temperatures and temp reading timeout
// temperatures are initialized @ 0C. At each unsuccesful reset or read attempt
// the value ONEWIRE_TEMP_FAIL is stored. This value is never the result of a succesful conversion
int temperatures[NR_OF_TEMP_SENSORS];

// Sensors are read in a circular manner. On e cycle completes in time equal to the conversion
// time. This variable holds the id of the sensor to be addressed next during the cycle.
unsigned char register_to_address;


/*
 * Pulsing output specific variables and defines
 */
unsigned char pulsing_duration;

/*
 * Functions of the device
 *
 * ONEWIRE devices specific section
 */

bool
set_temp_resolution_on_bus(unsigned char register_id, unsigned char resolution)
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
        ow_buf[i] = onewire_read_byte(pinmask);

      if (ow_buf[8] == calculate_onewire_crc(ow_buf, 8) && ow_buf[7] == 0x10)
        temperatures[register_id] = ow_buf[0] | (ow_buf[1] << 8);
      else
        temperatures[register_id] = ONEWIRE_TEMP_FAIL;
    }
}

// Return if conversion command is sent succesfully
// It takes a reference to a specific device and issues the convert command specificly for it
bool
issue_convert_for_device(unsigned char register_id)
{
  unsigned char pinmask = register_pinmask_map[register_id];

  if (onewire_reset(pinmask))
    {
      send_onewire_rom_commands(register_id);
      onewire_write_byte(CMD_CONVERT_T, pinmask);
      return TRUE;
    }
  return FALSE;
}


// Keep conversions going on for each sensor on the single bus there is on this device
void
operate_onewire_temp_measurement(void)
{
  unsigned char register_testmask;

  if (conv_complete)
    {
      // Not very nice to use a bitmap but we need to spare bytes for the stack to keep things safe:(
      // This bitmap is used to keep track of register conversion initiation information
      register_testmask = 0x01 << register_to_address;

      // Evaluate side effect: Only read until read is succesful
      if (register_conv_initiated & register_testmask)
          read_DS18xxx(register_to_address);

      register_conv_initiated &= ~register_testmask;
      if (issue_convert_for_device(register_to_address))
        register_conv_initiated |= register_testmask;

      // Reset the conversion timer and set the complete flag so we
      // can wait for conversion time expiry of the next device
      reset_timeout(TEMP_CONV_TIMER);
      conv_complete = FALSE;

      if(++register_to_address == NR_OF_TEMP_SENSORS)
        register_to_address = 0;
    }
  else
    {
      conv_complete = timeout_occured(TEMP_CONV_TIMER,
          (DS18x20_CONV_TIME / NR_OF_TEMP_SENSORS) + 50);
    }
}

/*
 * Define the functions of the pulse output
 */

// Pulse the output for pulse_time* 1/10 second

void operate_pulsing()
{
  // Return if pulsing is not active in any direction
  if (!(CCW_PIN || CW_PIN))
    return;

  // We know that one of the pulse outputs is active
  // Deactivate pulsing output if pulsing timeout occured
  if(timeout_occured(PULSING_OUTPUT_TIMER, pulsing_duration*100))
    {
      CCW_PIN = 0;
      CW_PIN = 0;
    }
}

bool start_output_pulse(direction_type direction, unsigned char pulse_time)
{
  // Return if pulsing is active in any direction
  if (CCW_PIN || CW_PIN)
    return FALSE;

  // Activate the proper output
  if (direction == DIRECTION_CW)
    {
      CW_PIN = 1;
    } else { // direction == DIRECTION_CCW
      CCW_PIN = 1;
    }

  reset_timeout(PULSING_OUTPUT_TIMER);
  pulsing_duration = pulse_time;
  return TRUE;
}

void
operate_device(void)
{

  // Messaging variables
  unsigned char response_opcode = RESPONSE_UNDEFINED, p;

  // The main loop of the device
  while (TRUE)
    {
      // Operate main device functions
      operate_onewire_temp_measurement();

      // Operate pulsing
      operate_pulsing();

      // Take care of messaging
      if (get_device_message() && !process_generic_messages())
        {
          // Set response opcode to undefined to filter response opcode programming issues
          response_opcode = RESPONSE_UNDEFINED;

          switch (message_buffer.content[OPCODE])
            {
          case SET_REGISTER:
            // p holds register to write
            p = message_buffer.content[PARAMETER_START];

            // Preset response opcode to success
            response_opcode = COMMAND_SUCCESS;

            switch (p)
            {
/*          Address 1:  Forward temp sensor
*           Address 2:  Incoming temp sensor
*           Address 3:  Return temp sensor
*           Address 4:  Upper Buffer temp sensor
*           Address 5:  Lower Buffer temp sensor
*/
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
              response_opcode = COMMAND_FAIL;
              break;

             // Address 6:  CW output - Contact CW
            case 6:
              if(!start_output_pulse(DIRECTION_CW, message_buffer.content[PARAMETER_START+1]))
                response_opcode = COMMAND_FAIL;
              break;
             // Address 7:  CCW output - Contact CCW
            case 7:
              if(!start_output_pulse(DIRECTION_CCW, message_buffer.content[PARAMETER_START+1]))
                response_opcode = COMMAND_FAIL;
              break;
            // Address 8:  SPA switch - Contact SPA
            case 8:
              SPAREA_PIN = (message_buffer.content[PARAMETER_START+1] > 0);
              break;
            // Address 9:  SPB switch - Contact SPB
            case 9:
              SPAREB_PIN = (message_buffer.content[PARAMETER_START+1] > 0);
              break;
            // Address 10:  GPIO - Contact 1 on the low power part
            case 10:
              GPIO_PIN = (message_buffer.content[PARAMETER_START+1] > 0);
              break;
            // Any other address fails
            default:
              response_opcode = COMMAND_FAIL;
              break;
            }

            message_buffer.index = PARAMETER_START-1;

            break;

          case READ_REGISTER:
            // p holds register to read
            p = message_buffer.content[PARAMETER_START];

            // Preset response opcode to success
            response_opcode = COMMAND_SUCCESS;

            switch (p)
            {
/*          Address 1:  Forward temp sensor
*           Address 2:  Incoming temp sensor
*           Address 3:  Return temp sensor
*           Address 4:  Upper Buffer temp sensor
*           Address 5:  Lower Buffer temp sensor
*/
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
              message_buffer.content[PARAMETER_START] = temperatures[p - 1] & 0x00ff;
              message_buffer.content[PARAMETER_START + 1] = (temperatures[p- 1] >> 8) & 0x00ff;
              message_buffer.index = PARAMETER_START + 1;
              break;

             // Address 6:  CW output - Contact CW
            case 6:
              message_buffer.content[PARAMETER_START] = CW_PIN;
              message_buffer.index = PARAMETER_START;
              break;
             // Address 7:  CCW output - Contact CCW
            case 7:
              message_buffer.content[PARAMETER_START] = CCW_PIN;
              message_buffer.index = PARAMETER_START;
              break;
            // Address 8:  SPA switch - Contact SPA
            case 8:
              message_buffer.content[PARAMETER_START] = SPAREA_PIN;
              message_buffer.index = PARAMETER_START;
              break;
            // Address 9:  SPB switch - Contact SPB
            case 9:
              message_buffer.content[PARAMETER_START] = SPAREB_PIN;
              message_buffer.index = PARAMETER_START;
              break;
            // Address 10:  GPIO - Contact 1 on the low power part
            case 10:
              message_buffer.content[PARAMETER_START] = GPIO_PIN;
              message_buffer.index = PARAMETER_START;
              break;
            // Any other address fails
            case 11:
              /* Test address to read rom on onewire bus - a single device should be connected to the bus in this case */
              onewire_reset(0x01);
              onewire_write_byte(CMD_READ_ROM, 0x01);

              for (p = 0; p < 8; p++)
                message_buffer.content[PARAMETER_START+p] =  onewire_read_byte(0x01);

              message_buffer.index = PARAMETER_START+7;
              break;
            default:
              response_opcode = COMMAND_FAIL;
              message_buffer.index = PARAMETER_START-1;
              break;
            }
            break;
          // Any other message code fails
          default:
            response_opcode = COMMAND_FAIL;
            message_buffer.index = PARAMETER_START-1;
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

  // Set initial resolutions to 12 bit
  set_temp_resolution_on_bus(0, TEMP_RESOLUTION_12BIT);

  for (i=0; i<NR_OF_TEMP_SENSORS; i++)
    temperatures[i] = 0;

  // Initialize bitfield - all conversions are uninitiated
  register_conv_initiated = 0;

  // We need to start a new conversion so it is complete on init
  conv_complete = TRUE;

  // Go for register #0 first
  register_to_address = 0;

  // Reset conversion timers and distribute conversion across the 3 sensors
  reset_timeout(TEMP_CONV_TIMER);


  SPAREA_PIN = SPAREB_PIN = CW_PIN = CCW_PIN = 0;
}

void
main(void)
{

  device_specific_init();

// Enable interrupts and initialize timer
  EA = 1;
  init_timer();

  init_device_comm(HOST_ID, COMM_SPEED_9600_H);

  operate_device();
}
