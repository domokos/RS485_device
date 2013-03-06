/*
 * Comm_test.c
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#include "Slave_comm.h"
#include "Onewire.h"
#include "Generic_device.h"

#define HOST_ID 1

#define TEMP1_PINMASK 0x01 // P1_0
#define TEMP2_PINMASK 0x02 // P1_1
#define TEMP3_PINMASK 0x04 // P1_2
// The masks used to manipulate temp timeout values
#define TEMP_TIMEOUT_MASK 0xE000
#define TEMP_TIMEOUT_INCREMENT 0x2000
#define TEMP_MASK 0x0FFF
#define TEMP_SIGN_MASK 0x1000
#define TEMP_ONE_SIGN 0xF000
#define TEMP_ZERO_SIGN 0x0FFF

#define TEMP_RESOLUTION_12BIT 0x7F

__code const char register_identification[][REG_IDENTIFICATION_LEN] =
  {
    { REG_TYPE_TEMP, REG_RW, 2, 0, 1 }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
        { REG_TYPE_TEMP, REG_RW, 2, 0, 1 }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
        { REG_TYPE_TEMP, REG_RW, 2, 0, 1 }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
        { REG_TYPE_SW, REG_RW, 1, 0, 0 } };

bool wait_conv, temp1_conv_initiated, temp2_conv_initiated,
    temp3_conv_initiated;

// Buffer to store Temperatures and temp reading timeout
// temperatures are initialized @ 0C. At each unsuccesful reset attempt
// the unused upper 3 bits are increased. After reaching -1000 the reset attempt rate is decreased
// and is done at every 5th cyce (~4 seconds) then reaching -10 000 reset attemts are made every 10th cycle (~8 seconds)
int temperatures[3];
unsigned char next_sensor;

bool
set_temp_resolution(unsigned char pinmask, unsigned char resolution)
{
  bool success;

  if (onewire_reset(pinmask))
    {
      onewire_write_byte(CMD_WRITE_SCRATCHPAD, pinmask);
      onewire_write_byte(0, pinmask);
      onewire_write_byte(0, pinmask);
      onewire_write_byte(resolution, pinmask);
      success = TRUE;
    }
  else
    {
      success = FALSE;
    }
  return success;
}

void
scale_DS18B20_result(unsigned char id)
{
  temperatures[id] *= 8;
  temperatures[id] &= 0xFFF0;
  temperatures[id] += 12;
  temperatures[id] -= ow_buf[6];
}

// Calculate Which pin is referenced in the pinmask
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
        temperatures[id] = ow_buf[0] | (ow_buf[1] << 8);

      // If result needs scaling up then scale it up
      if (register_identification[id][3])
        scale_DS18B20_result(id);
    }
}

// Return if conversion initiated succesfully
bool
issue_convert(unsigned char pinmask)
{
  unsigned char id;
  bool conv_initiated;

  id = get_id(pinmask);

  if (onewire_reset(pinmask))
    {
      onewire_write_byte(CMD_SKIP_ROM, pinmask);
      onewire_write_byte(CMD_CONVERT_T, pinmask);
      conv_initiated = TRUE;
    }
  else
    {
      if ((temperatures[id] & TEMP_TIMEOUT_MASK) < TEMP_TIMEOUT_MASK
          && (temperatures[id] & TEMP_TIMEOUT_MASK) != 0)
        temperatures[id] = (temperatures[id] & TEMP_MASK)
            | ((temperatures[id] & TEMP_TIMEOUT_MASK) + TEMP_TIMEOUT_INCREMENT);
      conv_initiated = FALSE;
    }
  return conv_initiated;
}

// Keep conversions going on for each sensor attached
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
        next_sensor = 3;
        break;

      case 3:
        if (temp3_conv_initiated)
          read_DS18xxx(TEMP3_PINMASK);
        temp3_conv_initiated = issue_convert(TEMP3_PINMASK);
        next_sensor = 1;
        break;
        }
      reset_timeout(TEMP_CONV_TIMER);
      wait_conv = TRUE;
    }
  else
    {
      wait_conv = !timeout_occured(TEMP_CONV_TIMER, DS18x20_CONV_TIME / 3);
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
 */
void
operate_device(void)
{

  unsigned int nr_calls = 0, temp;
  unsigned char timeout, response_opcode, p;

  timeout = get_messaging_timeout();
  reset_timeout(RESPONSE_TIMEOUT);

  while (TRUE)
    {
      operate_onewire();

      if (get_device_message() && !process_generic_messages())
        {
          switch (message_buffer.content[OPCODE])
            {
          case SET_REGISTER:
            p = 0x01 << message_buffer.content[PARAMETER_START] - 1;
            if (message_buffer.content[PARAMETER_START + 1])
              P1 |= p;
            else
              P1 &= ~p;
            response_opcode = COMMAND_SUCCESS;
            break;
          case READ_REGISTER:
            // Switch register number
            if ((p = message_buffer.content[PARAMETER_START] < 4))
              {
                temp = temperatures[p - 1];

                if (temp & TEMP_SIGN_MASK)
                  temp = temp | TEMP_ONE_SIGN;
                else
                  temp = temp & TEMP_ZERO_SIGN;
                message_buffer.content[PARAMETER_START] = temp & 0x00ff;
                message_buffer.content[PARAMETER_START + 1] = (temp >> 8)
                    & 0x00ff;
                message_buffer.content[PARAMETER_START + 2] = (temperatures[p
                    - 1] & TEMP_TIMEOUT_MASK) >> 13;
                message_buffer.index = PARAMETER_START + 2;

                if ((temp & TEMP_TIMEOUT_MASK) < TEMP_TIMEOUT_MASK
                    && (temp & TEMP_TIMEOUT_MASK) != 0)
                  response_opcode = COMMAND_FAIL;
                else
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
  int i;
// Initialize to zero
  for (i = 0; i < 3; i++)
    temperatures[i] = 0;

// Set initial resolutions to 12 bit
  set_temp_resolution(TEMP_RESOLUTION_12BIT, TEMP1_PINMASK);
  set_temp_resolution(TEMP_RESOLUTION_12BIT, TEMP1_PINMASK);
  set_temp_resolution(TEMP_RESOLUTION_12BIT, TEMP1_PINMASK);

  wait_conv = FALSE;

  temp1_conv_initiated = temp2_conv_initiated = temp3_conv_initiated = FALSE;
  next_sensor = 1;

// Reset conversion timers and distribute conversion across the 3 sensors
  reset_timeout(TEMP_CONV_TIMER);
}

void
main(void)
{
// Enable interrupts
  EA = 1;

// Set 4800 baud @ 11.0592 MHz Crystal
  init_device_comm(HOST_ID, COMM_SPEED_4800_H);

// onewire_test();
  device_specific_init();

  operate_device();

}
