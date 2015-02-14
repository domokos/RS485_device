/*
 * Furnace_temp_device.c
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#include "Generic_device.h"
#include "Main_panel_device.h"

/*
 * Define registers of this device
 */
// This device has 11 registers
__code const unsigned char nr_of_registers = 11;

#define NR_OF_TEMP_SENSORS 4
#define NR_OF_OW_BUSES 1

// Describe the registers of this device
__code const unsigned char register_identification[][REG_IDENTIFICATION_LEN] =
  {
      // HW temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, SCALE_TEMP, FIXED_RESOLUTION }, // DS18S20 - value1: scaling is needed(1), value2: fixed reesolution(0)
      // Basement temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, SCALE_TEMP, FIXED_RESOLUTION }, // DS18S20 - value1: scaling is needed(1), value2: fixed resolution(0)
      // Return temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Hidr Shift temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)

      // Extender register outputs

      // Radiator pump - Extender bit 0 - Q0 - Contact 4
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Floor pump - Extender bit 1 - Q1 - Contact 5
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Hidraulic Shifter pump - Extender bit 2 - Q2 - Contact 6
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // HW pump - Extender bit 3 - Q3 - - Contact 7
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Basement floor valve - Extender bit 4 - Q4 - Contact 9
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Basement radiator valve - Extender bit 5 - Q5 - - Contact 8
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Buffertop valve - Extender bit 6 - Q6 - Contact 3
         { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Return valve - Extender bit 7 - Q7 - - Contact 2
         { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },

      // Heater relay P3_5
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },

      // Furnace temp wiper
        { REG_TYPE_DATA, REG_RW, 1, DONT_CARE, DONT_CARE },

      // HW temp wiper
        { REG_TYPE_DATA, REG_RW, 1, DONT_CARE, DONT_CARE } };


/*
 * Onewire specific declarations and defines
 */
// Map registers to onewire buses all registers are P3_3
__code const unsigned char register_pinmask_map[4] =
  {0x04, 0x04, 0x04, 0x04};

// Store 64 bit rom values of registers/devices
__code const unsigned char register_rom_map[][8] =
  {
  // If the first byte is zero, then there is only one device on bus
      // The HW sesor
        { 0x10, 0xc7, 0xa2, 0x23, 0x01, 0x08, 0x00, 0x74 },
      // The Basement sensor
        { 0x10, 0x47, 0xbf, 0x24, 0x01, 0x08, 0x00, 0x88 },
      // The return water temp sensor
        { 0x28, 0x91, 0x2d, 0x50, 0x01, 0x00, 0x00, 0xff },
      // The Hidr Shift water temp sensor
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
scale_DS18S20_result(unsigned char register_id)
{
  temperatures[register_id] &= 0xFFFE;
  temperatures[register_id] *= 8;
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
        ow_buf[i] = onewire_read_byte(pinmask);

      if (ow_buf[8] == calculate_onewire_crc(ow_buf, 8) && ow_buf[7] == 0x10)
        {
          temperatures[register_id] = ow_buf[0] | (ow_buf[1] << 8);

          // If result needs scaling up then scale it up
          if (register_identification[register_id][SCALE_POSITION] == SCALE_TEMP)
            scale_DS18S20_result(register_id);
        }
      else
        {
          temperatures[register_id] = ONEWIRE_TEMP_FAIL;
        }
    }
}

#if 0 // We do not use this
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
#endif

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

  // Messaging variables
  unsigned char response_opcode = RESPONSE_UNDEFINED, p;

  bool got_message;

  // The main loop of the device
  while (TRUE)
    {
      // Operate main device functions
      operate_onewire_temp_measurement();

      got_message = get_device_message();

/*
 *    Watch communication activity on bus and reset the device outputs
 *    if no communication is seen whithin timeout
 */
      if (!got_message)
        {
          if (timeout_occured(BUS_COMMUNICATION_WATCHDOG_TIMER, BUS_COMMUNICATION_TIMEOUT_MS))
          {
            // Re-initialize the device - shut every output down
            device_specific_init_phase1();
            init_device_comm(HOST_ID, COMM_SPEED_9600_H);
            device_specific_init_phase2();

            // Reset the timer
            reset_timeout(BUS_COMMUNICATION_WATCHDOG_TIMER);
          }
        } else {
          reset_timeout(BUS_COMMUNICATION_WATCHDOG_TIMER);
        }

      // Take care of messaging
      if (got_message && !process_generic_messages())
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

            if ( p>0 && p<=4 )
              {
/*          Address 1:  HW temp sensor
*           Address 2:  Basement temp sensor
*           Address 3:  Return temp sensor
*           Address 4:  Hidr Shift temp sensor
*/
              response_opcode = COMMAND_FAIL;
              } else if( p <= 12 ) {
/*
*           Address 5:  Buffertop valve - Contact 2
*           Address 6:  Return valve - Contact 3
*           Address 7:  Radiator pump - Contact 4
*           Address 8:  Floor pump - Contact 5
*           Address 9:  Hidraulic Shifter pump - Contact 6
*           Address 10:  HW pump - Contact 7
*           Address 11:  Basement radiator valve Contact 8
*           Address 12:  Basement floor valve - Contact 9
*/
               write_extender(p-5, message_buffer.content[PARAMETER_START + 1] > 0);
              } else if(p == 13) {
/*          Address 13: Heater relay P3_5 - Heater contact */
              HEATER_RELAY_PIN = message_buffer.content[PARAMETER_START + 1] > 0;
              } else if(p <= 15) {
/*
*           Temp wipers - expected data format:
*           Byte 1 & 2 - 9 bits of data holdiong the desired wiper setting
*           Byte 3 - bool flag - is volatile
*           Address 14: HW Wiper
*           Address 15: Heating Wiper
*/
              if (!write_wiper(
                  (message_buffer.content[PARAMETER_START+1] << 8) | message_buffer.content[PARAMETER_START+2] ,
                  message_buffer.content[PARAMETER_START + 3]>0,
                  p == 14 ? WIPER_HW : WIPER_HEAT))
                response_opcode = COMMAND_FAIL;
              } else {
/*          Any other address fails */
              response_opcode = COMMAND_FAIL;
            }

            message_buffer.index = PARAMETER_START-1;

            break;

          case READ_REGISTER:
            // p holds register to read
            p = message_buffer.content[PARAMETER_START];

            // Preset response opcode to success
            response_opcode = COMMAND_SUCCESS;

            if ( p>0 && p<=4 )
              {
/*          Address 1:  HW temp sensor
*           Address 2:  Basement temp sensor
*           Address 3:  Return temp sensor
*           Address 4:  Hidr Shift temp sensor
*/
              message_buffer.content[PARAMETER_START] = temperatures[p - 1] & 0x00ff;
              message_buffer.content[PARAMETER_START + 1] = (temperatures[p- 1] >> 8) & 0x00ff;
              message_buffer.index = PARAMETER_START + 1;
              } else if( p <= 12 ) {
/*
*           Address 5:  Buffertop valve - Contact 2
*           Address 6:  Return valve - Contact 3
*           Address 7:  Radiator pump - Contact 4
*           Address 8:  Floor pump - Contact 5
*           Address 9:  Hidraulic Shifter pump - Contact 6
*           Address 10:  HW pump - Contact 7
*           Address 11:  Basement radiator valve Contact 8
*           Address 12:  Basement floor valve - Contact 9
*/
              message_buffer.content[PARAMETER_START] = get_extender_value(p-5);
              message_buffer.index = PARAMETER_START;
              } else if(p == 13) {
 /*          Address 13: Heater relay P3_5 - Heater contact */
              message_buffer.content[PARAMETER_START] = HEATER_RELAY_PIN;
              message_buffer.index = PARAMETER_START;
              } else if(p <= 15) {
/*
*             Temp wipers - expected data format:
*             Byte 1 - bool flag - is volatile
*             Address 14: HW Wiper
*             Address 15: Heating Wiper
*/
              read_wiper((unsigned int*)(message_buffer.content+PARAMETER_START), message_buffer.content[PARAMETER_START+1]>0, p == 14 ? WIPER_HW : WIPER_HEAT);
              message_buffer.index = PARAMETER_START+1;
              } else if(p == 16) {
/* Test address to read rom on onewire bus - a single device should be connected to the bus in this case */

              onewire_reset(0x04);
              onewire_write_byte(CMD_READ_ROM, 0x04);

              for (p = 0; p < 8; p++)
                message_buffer.content[PARAMETER_START+p] =  onewire_read_byte(0x04);

              message_buffer.index = PARAMETER_START+7;
              } else {
              response_opcode = COMMAND_FAIL;
              message_buffer.index = PARAMETER_START-1;
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
device_specific_init_phase1(void)
{
  // Turn off all outputs
  reset_extender();
  HEATER_RELAY_PIN = 0;

  // Reset the MCP4161 rheostat
  reset_rheostats();
}

void
device_specific_init_phase2(void)
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
}

void
main(void)
{

  device_specific_init_phase1();

// Enable interrupts and initialize timer
  EA = 1;
  init_timer();

  init_device_comm(HOST_ID, COMM_SPEED_9600_H);

  device_specific_init_phase2();

  operate_device();
}
