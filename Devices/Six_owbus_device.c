/*
 * Six_owbus_device.c
 *
 *  Created on: Aug 23, 2013
 *      Author: dmolnar
 */

#include "Slave_comm.h"
#include "Onewire.h"
#include "Generic_device.h"

// The id of this host on the bus
#define HOST_ID 10

__code const unsigned char nr_of_registers = 8;

#define NR_OF_TEMP_SENSORS 6
#define NR_OF_OW_BUSES 6
#define SINGLE_DEVICE_ON_BUS 0

// Describe the registers of this device
__code const unsigned char register_identification[][REG_IDENTIFICATION_LEN] =
  {
  // Furnace temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // HW temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Basement temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Return temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Floor water temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Spare1 temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION } // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
  };

/*
 * Onewire specific declarations and defines
 */
// Map registers to onewire buses: Register N is on P1_N
__code const unsigned char register_pinmask_map[6] =
  { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20 };

// Store 64 bit rom values of registers/devices
__code const unsigned char register_rom_map[][8] =
  {
  // First byte is zero, only one device on bus other chars are ignored
        { SINGLE_DEVICE_ON_BUS, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      // First byte is zero, only one device on bus other chars are ignored
        { SINGLE_DEVICE_ON_BUS, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      // First byte is zero, only one device on bus other chars are ignored
        { SINGLE_DEVICE_ON_BUS, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      // First byte is zero, only one device on bus other chars are ignored
        { SINGLE_DEVICE_ON_BUS, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      // First byte is zero, only one device on bus other chars are ignored
        { SINGLE_DEVICE_ON_BUS, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      // First byte is zero, only one device on bus
        { SINGLE_DEVICE_ON_BUS, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

bool conv_complete;
bool bus0_conv_initiated, bus1_conv_initiated, bus2_conv_initiated;
bool bus3_conv_initiated, bus4_conv_initiated, bus5_conv_initiated;

// Buffer to store Temperatures and temp reading timeout
// temperatures are initialized @ 0C. At each unsuccesful reset or read attempt
// the value ONEWIRE_TEMP_FAIL is stored. This value is never the result of a succesful conversion
int temperatures[NR_OF_TEMP_SENSORS];

// Sensors are read in a circular manner. One cycle completes in time equal to the conversion
// time. This variable holds the id of the sensor to be addressed next during the cycle.
unsigned char bus_to_address;

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
  conv_complete |= timeout_occured(TEMP_CONV_TIMER,
      DS18x20_CONV_TIME / NR_OF_OW_BUSES);

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
        bus_to_address = 2;
        break;
      case 2:
        if (bus2_conv_initiated)
          read_DS18xxx(2);
        bus2_conv_initiated = issue_convert_on_bus(2);
        bus_to_address = 3;
        break;
      case 3:
        if (bus3_conv_initiated)
          read_DS18xxx(3);
        bus3_conv_initiated = issue_convert_on_bus(3);
        bus_to_address = 4;
        break;
      case 4:
        if (bus4_conv_initiated)
          read_DS18xxx(4);
        bus4_conv_initiated = issue_convert_on_bus(4);
        bus_to_address = 5;
        break;
      case 5:
        if (bus5_conv_initiated)
          read_DS18xxx(5);
        bus5_conv_initiated = issue_convert_on_bus(5);
        bus_to_address = 0;
        break;
        }

      // Reset the conversion timer and clear the complete flag so we
      // can wait for conversion time expiry on the next bus
      reset_timeout(TEMP_CONV_TIMER);
      conv_complete = FALSE;
    }
}

void
operate_device(void)
{

  // Create messaging variables
  unsigned char response_opcode = RESPONSE_UNDEFINED, p, pinmask;

  // The main loop of the device
  while (TRUE)
    {
      // Operate main device functions
      operate_onewire_temp_measurement();

      // Take care of messaging
      if (get_device_message() && !process_generic_messages())
        {
          switch (message_buffer.content[OPCODE])
            {
          case SET_REGISTER:
            response_opcode = COMMAND_FAIL;
            break;
          case READ_REGISTER:
            // p holds register to read
            p = message_buffer.content[PARAMETER_START];
            // Branch based on register number
            // The first 6 registers read temperatures
            if (p < 7)
              {
                message_buffer.content[PARAMETER_START] = temperatures[p - 1]
                    & 0x00ff;
                message_buffer.content[PARAMETER_START + 1] = (temperatures[p
                    - 1] >> 8) & 0x00ff;
                message_buffer.index = PARAMETER_START + 1;

                response_opcode = COMMAND_SUCCESS;
              }
            // The next 6 registers return ROM values of sensors attached
            else if (p < 13)
              {
                pinmask = register_pinmask_map[p - 6];
                onewire_reset(pinmask);
                onewire_write_byte(CMD_READ_ROM, pinmask);

                p = 0;
                do
                  {
                    message_buffer.content[PARAMETER_START + p] =
                        onewire_read_byte(pinmask);
                  }
                while (++p < 8);
                message_buffer.index = PARAMETER_START + 7;

                if (message_buffer.content[PARAMETER_START + p]
                    == calculate_onewire_crc(
                        message_buffer.content + PARAMETER_START, 8))
                  response_opcode = COMMAND_SUCCESS;
                else
                  response_opcode = COMMAND_FAIL;
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
  set_temp_resolution(2, TEMP_RESOLUTION_12BIT);
  set_temp_resolution(3, TEMP_RESOLUTION_12BIT);
  set_temp_resolution(4, TEMP_RESOLUTION_12BIT);
  set_temp_resolution(5, TEMP_RESOLUTION_12BIT);

  // We need to start a new conversion so it is complete on init
  conv_complete = TRUE;

  bus0_conv_initiated = bus1_conv_initiated = bus2_conv_initiated = FALSE;
  bus3_conv_initiated = bus4_conv_initiated = bus5_conv_initiated = FALSE;
  bus_to_address = 0;

  // Reset conversion timers and distribute conversion across the 3 sensors
  reset_timeout(TEMP_CONV_TIMER);
}

void
main(void)
{
// Enable interrupts and initialize timer
  EA = 1;
  init_timer();

// Set 4800 baud
  init_device_comm(HOST_ID, COMM_SPEED_4800_H);

  device_specific_init();

  operate_device();

}
