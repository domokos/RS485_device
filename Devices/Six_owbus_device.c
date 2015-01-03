/*
 * Six_owbus_device.c
 *
 *  Created on: Aug 23, 2013
 *      Author: dmolnar
 */

#include "Slave_comm.h"
#include "Onewire.h"
#include "Generic_device.h"

void device_specific_init();

// The id of this host on the bus
#define HOST_ID 10

__code const unsigned char nr_of_registers = 8;

#define NR_OF_TEMP_SENSORS 3
#define SINGLE_DEVICE_ON_BUS 0

// Describe the registers of this device
__code const unsigned char register_identification[][REG_IDENTIFICATION_LEN] =
  {
      // External temp sensor - Bus3
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Living temp sensor - Bus3
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Upstairs temp sensor - Bus2
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Living floor valve - Bus1
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE }, // DS2405
      // Upstairs floor valve - Bus2
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE } // DS2405
  };

/*
 * Onewire specific declarations and defines
 */
// Map registers to onewire buses: Register N is on P1_N
__code const unsigned char register_pinmask_map[5] =
  { 0x04, 0x04, 0x02, 0x01, 0x02};

// Store 64 bit rom values of registers/devices
__code const unsigned char register_rom_map[][8] =
  {
      // External temp sensor - 28CBD248010000F3 - Bus3
        { 0x28, 0xcb, 0xd2, 0x48, 0x01, 0x00, 0x00, 0xf3 },
      // Living temp sensor - 280AEA48010000E1 - Bus3
        { 0x28, 0x0a, 0xea, 0x48, 0x01, 0x00, 0x00, 0xe1 },
      // Upstairs temp sensor - 286A0B5001000011 - Bus2
        { 0x28, 0x6a, 0x0b, 0x50, 0x01, 0x00, 0x00, 0x11 },
      // Living floor valve - 055E053200000069 - Bus1
//        { 0x05, 0x5e, 0x05, 0x32, 0x00, 0x00, 0x00, 0x69 },
//        Test device: 05 CE E1 31 000000 95
        { 0x05, 0xce, 0xe1, 0x31, 0x00, 0x00, 0x00, 0x95 },
      // Upstairs floor valve - 05200232000000B3 - Bus2
        { 0x05, 0x20, 0x02, 0x32, 0x00, 0x00, 0x00, 0xb3 } };

bool conv_complete;

// Buffer to store Temperatures and temp reading timeout
// temperatures are initialized @ 0C. At each unsuccesful reset or read attempt
// the value ONEWIRE_TEMP_FAIL is stored. This value is never the result of a succesful conversion
int temperatures[NR_OF_TEMP_SENSORS];

// Sensors are read in a circular manner. One cycle completes in time equal to the conversion
// time. This variable holds the id of the sensor to be addressed next during the cycle.
unsigned char register_to_address, register_conv_initiated;

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
  if (register_rom_map[register_id][0] == SINGLE_DEVICE_ON_BUS)
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

#if 0 // Not used now
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

// Keep conversions going on for each sensor on each onewire bus
void
operate_onewire_temp_measurement(void)
{
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
}

void
operate_device(void)
{

  // Create messaging variables
  unsigned char response_opcode = RESPONSE_UNDEFINED, p, pinmask;

  bool got_message;

  got_message = get_device_message();
#if 0
  /*
  *    Watch communication activity on bus and reset the device outputs
  *    if no communication is seen whithin timeout
  */
  if (!got_message)
    {
      if (timeout_occured(BUS_COMMUNICATION_WATCHDOG_TIMER, BUS_COMMUNICATION_TIMEOUT_MS))
      {
        // Re-initialize the device - shut every output down
        device_specific_init();
        init_device_comm(HOST_ID, COMM_SPEED_9600_H);

        // Reset the timer
        reset_timeout(BUS_COMMUNICATION_WATCHDOG_TIMER);
      }
    } else {
      reset_timeout(BUS_COMMUNICATION_WATCHDOG_TIMER);
    }
#endif

  // The main loop of the device
  while (TRUE)
    {
      // Operate main device functions
      operate_onewire_temp_measurement();

      // Take care of messaging
      if (got_message && !process_generic_messages())
        {
          // Set response opcode to undefined to ensure issue identification
          response_opcode = RESPONSE_UNDEFINED;

          switch (message_buffer.content[OPCODE])
            {
          case SET_REGISTER:
            // p holds register to write
            p = message_buffer.content[PARAMETER_START];

            // Preset response opcode to success
            response_opcode = COMMAND_SUCCESS;

            if(p>0 && p<3)
              {
/*          Address 1:  External temp sensor
*           Address 2:  Living temp sensor
*           Address 3:  Upstairs temp sensor
*/
                // Read-only temp registers - command fails
                response_opcode = COMMAND_FAIL;
              } else if( p<6 ) {
/*          Address 4:  Living floor valve
*           Address 5:  Upstairs floor valve
*/
              pinmask = register_pinmask_map[p-1];

              if (onewire_reset(pinmask))
                {
                  // If the value read and the value got on the bus do not equal then toggle the value of the DS2405 switch
                  if((message_buffer.content[PARAMETER_START + 1] > 0) != ReadDS2405(register_rom_map[p-1], pinmask))
                    send_onewire_rom_commands(p-1);
                } else {
                  response_opcode = COMMAND_FAIL;
                }
              } else {
                response_opcode = COMMAND_FAIL;
              }
            message_buffer.index = PARAMETER_START-1;
            break;
          case READ_REGISTER:
            // p holds register to write
            p = message_buffer.content[PARAMETER_START];

            // Preset response opcode to success
            response_opcode = COMMAND_SUCCESS;

            if(p>0 && p<3)
              {
/*          Address 1:  External temp sensor
*           Address 2:  Living temp sensor
*           Address 3:  Upstairs temp sensor
*/
                message_buffer.content[PARAMETER_START] = temperatures[p - 1] & 0x00ff;
                message_buffer.content[PARAMETER_START + 1] = (temperatures[p- 1] >> 8) & 0x00ff;
                message_buffer.index = PARAMETER_START + 1;
              } else if( p<6 ) {
/*          Address 4:  Living floor valve
*           Address 5:  Upstairs floor valve
*/
              pinmask = register_pinmask_map[p-1];
              if (onewire_reset(pinmask))
                {
                  message_buffer.content[PARAMETER_START] = ReadDS2405(register_rom_map[p-1], pinmask);
                  message_buffer.index = PARAMETER_START;
                } else {
                  response_opcode = COMMAND_FAIL;
                  message_buffer.index = PARAMETER_START-1;
                }
              } else if (p == 6){
                  pinmask = register_pinmask_map[0];

                  onewire_reset(pinmask);
                  onewire_write_byte(CMD_READ_ROM, pinmask);

                  for (p = 0; p < 8; p++)
                    message_buffer.content[PARAMETER_START+p] =  onewire_read_byte(pinmask);

                  message_buffer.index = PARAMETER_START+7;
              } else {
                response_opcode = COMMAND_FAIL;
                message_buffer.index = PARAMETER_START-1;
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
  unsigned char i, pinmask;

  i = NR_OF_TEMP_SENSORS;
  while (i--)
    temperatures[i - 1] = 0;

  // Set initial resolutions to 12 bit
  set_temp_resolution(0, TEMP_RESOLUTION_12BIT);
  set_temp_resolution(1, TEMP_RESOLUTION_12BIT);
  set_temp_resolution(2, TEMP_RESOLUTION_12BIT);

  // We need to start a new conversion so it is complete on init
  conv_complete = TRUE;

  // Reset conversion timers and distribute conversion across the 3 sensors
  reset_timeout(TEMP_CONV_TIMER);

  // Reset outputs
  for(i=3;i<5;i++)
    {
    pinmask = register_pinmask_map[i];

    if (onewire_reset(pinmask))
      {
        // If the value read and the value got on the bus do not equal then toggle the value of the DS2405 switch
        if(ReadDS2405(register_rom_map[i], pinmask))
          send_onewire_rom_commands(i);
      }
    }
}

void
main(void)
{
// Enable interrupts and initialize timer
  EA = 1;
  init_timer();

// Set 9600 baud
  init_device_comm(HOST_ID, COMM_SPEED_9600_H);

  device_specific_init();

  operate_device();

}
