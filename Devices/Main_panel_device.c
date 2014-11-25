/*
 * Furnace_temp_device.c
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#include "Generic_device.h"
#include "Main_panel_device.h"

// The id of this host on the bus
#define HOST_ID 11

/*
 * Define registers of this device
 */
// This device has 11 registers
__code const unsigned char nr_of_registers = 11;

#define NR_OF_TEMP_SENSORS 3
#define NR_OF_OW_BUSES 1

// Describe the registers of this device
__code const unsigned char register_identification[][REG_IDENTIFICATION_LEN] =
  {
      // HW temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Return temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Basement temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)

      // Single pin outputs

      // Radiator pump P1_4
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Floor pump P1_5
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Hidraulic Shifter pump P1_3
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // HW pump P1_6
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Basement floor valve P1_2
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Basement radiator valve P1_1
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },

      // Heater relay P3_5
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },

      // Furnace temp wiper
        { REG_TYPE_DATA, REG_RW, 1, DONT_CARE, DONT_CARE } };
/*
 * Onewire specific declarations and defines
 */
// Map registers to onewire buses all registers are P3_3
__code const unsigned char register_pinmask_map[1] =
  { 0x04};

// Store 64 bit rom values of registers/devices
__code const unsigned char register_rom_map[][8] =
  {
  // If the first byte is zero, then there is only one device on bus

      // TODO: Get the real sensor ID's for the device
        { 0x28, 0xe8, 0x33, 0x50, 0x01, 0x00, 0x00, 0x2f },
      // TODO: Get the real sensor ID's for the device
        { 0x28, 0x5f, 0xfb, 0x4f, 0x01, 0x00, 0x00, 0x13 },
      // TODO: Get the real sensor ID's for the device
        { 0x00, 0x5f, 0xfb, 0x4f, 0x01, 0x00, 0x00, 0x13 } };

bool conv_complete, bus0_conv_initiated;

// Buffer to store Temperatures and temp reading timeout
// temperatures are initialized @ 0C. At each unsuccesful reset or read attempt
// the value ONEWIRE_TEMP_FAIL is stored. This value is never the result of a succesful conversion
int temperatures[NR_OF_TEMP_SENSORS];

// Sensors are read in a circular manner. On e cycle completes in time equal to the conversion
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
        // Evaluate side effect: Only read until read is succesful
        if (bus0_conv_initiated)
          {
            read_DS18xxx(0);
            read_DS18xxx(1);
            read_DS18xxx(2);
          }
        bus0_conv_initiated = issue_convert_on_bus(0);
// Only single bus on this device so there is no need to switch between buses
//        bus_to_address = 1;
        break;
// There is only a single onewire bus on this device
#if 0
      case 1:
        // Evaluate side effect: Only read until read is succesful
        if (bus1_conv_initiated)
          {
            read_DS18xxx(2);
          }
        bus1_conv_initiated = issue_convert_on_bus(2);
        bus_to_address = 0;
        break;
#endif
        }

      // Reset the conversion timer and set the complete flag so we
      // can wait for conversion time expiry on the next bus - if there is one
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
            // p holds register to write
            p = message_buffer.content[PARAMETER_START];

            // Preset response opcode to success
            response_opcode = COMMAND_SUCCESS;

            switch (p)
            {
            // HW temp sensor
            case 1:
            // Return temp sensor
            case 2:
            // Basement temp sensor
            case 3:
              response_opcode = COMMAND_FAIL;
              break;
            // Radiator pump P1_4
            case 4:
              P1_4 = message_buffer.content[PARAMETER_START + 1];
              break;
            // Floor pump P1_5
            case 5:
              P1_5 = message_buffer.content[PARAMETER_START + 1];
              break;
            // Hidraulic Shifter pump P1_3
            case 6:
              P1_3 = message_buffer.content[PARAMETER_START + 1];
              break;
            // HW pump P1_6
            case 7:
              P1_6 = message_buffer.content[PARAMETER_START + 1];
              break;
            // Basement floor valve P1_2
            case 8:
              P1_2 = message_buffer.content[PARAMETER_START + 1];
              break;
            // Basement radiator valve P1_1
            case 9:
              P1_1 = message_buffer.content[PARAMETER_START + 1];
              break;
            // Heater relay P3_5
            case 10:
              P3_5 = message_buffer.content[PARAMETER_START + 1];
              break;
            // Furnace temp wiper - expected data format:
            // Byte 1 & 2 - 9 bits of data holdiong the desired wiper setting
            // Byte 3 - bool flag - is volatile
            case 11:
              write_wiper( *(message_buffer.content + PARAMETER_START + 1), message_buffer.content[PARAMETER_START + 2]);
              break;
            // Any other address fails
            default:
              response_opcode = COMMAND_FAIL;
              break;
            }
            break;

          case READ_REGISTER:
            // p holds register to read
            p = message_buffer.content[PARAMETER_START];

            switch (p)
            {
            // HW temp sensor
            case 1:
            // Return temp sensor
            case 2:
            // Basement temp sensor
            case 3:
              message_buffer.content[PARAMETER_START] = temperatures[p - 1]
                  & 0x00ff;
              message_buffer.content[PARAMETER_START + 1] = (temperatures[p
                  - 1] >> 8) & 0x00ff;
              message_buffer.index = PARAMETER_START + 1;
              break;
            // Radiator pump P1_4
            case 4:
              message_buffer.content[PARAMETER_START] = P1_4;
              message_buffer.index = PARAMETER_START;
              break;
            // Floor pump P1_5
            case 5:
              message_buffer.content[PARAMETER_START] = P1_5;
              message_buffer.index = PARAMETER_START;
              break;
            // Hidraulic Shifter pump P1_3
            case 6:
              message_buffer.content[PARAMETER_START] = P1_3;
              message_buffer.index = PARAMETER_START;
              break;
            // HW pump P1_6
            case 7:
              message_buffer.content[PARAMETER_START] = P1_6;
              message_buffer.index = PARAMETER_START;
              break;
            // Basement floor valve P1_2
            case 8:
              message_buffer.content[PARAMETER_START] = P1_2;
              message_buffer.index = PARAMETER_START;
              break;
            // Basement radiator valve P1_1
            case 9:
              message_buffer.content[PARAMETER_START] = P1_1;
              message_buffer.index = PARAMETER_START;
              break;
            // Heater relay P3_5
            case 10:
              message_buffer.content[PARAMETER_START] = P3_5;
              message_buffer.index = PARAMETER_START;
              break;
            // Furnace temp wiper - expected data format:
            // Byte 1 - bool flag - is volatile
            case 11:
              read_wiper((unsigned int*)(message_buffer.content+PARAMETER_START), message_buffer.content[PARAMETER_START]);
              message_buffer.index = PARAMETER_START+1;
              break;
            // Any other address fails
            default:
              response_opcode = COMMAND_FAIL;
              break;
            }
            break;

          // Any other massage code fails
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
    {
    temperatures[i - 1] = 0;

    // Set initial resolutions to 12 bit
    set_temp_resolution(i-1, TEMP_RESOLUTION_12BIT);
    }

  // We need to start a new conversion so it is complete on init
  conv_complete = TRUE;

  bus0_conv_initiated = FALSE;
  bus_to_address = 0;

  // Reset conversion timers and distribute conversion across the 3 sensors
  reset_timeout(TEMP_CONV_TIMER);

  // Turn off all outputs
  P3_5=P1_1=P1_2=P1_3=P1_4=P1_5=P1_6=0;

  // Reset the MCP4161 rheostat
  rheostat_reset();
}

void
main(void)
{
// Enable interrupts and initialize timer
  EA = 1;
  init_timer();

  init_device_comm(HOST_ID, COMM_SPEED_9600_H);

  device_specific_init();

  operate_device();
}
