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

#define TEMP1_PINMASK 0x01
#define TEMP2_PINMASK 0x02
#define TEMP3_PINMASK 0x04


__code const char register_identification[][REG_IDENTIFICATION_LEN] = {
   {REG_TYPE_TEMP, REG_RO, 9, 0, 0},
   {REG_TYPE_SW, REG_RW, 1, 0, 0},
   {REG_TYPE_TEMP, REG_RO, 9, 0, 0},
   {REG_TYPE_SW, REG_RW, 1, 0, 0}
};

bool conv_1_active, conv_2_active, conv_3_active;
int temperatures[3];

void read_DS18S20(unsigned char id)
{
  unsigned char pinmask=0, i;
  switch(id)
    {
    case 1:
      pinmask = TEMP1_PINMASK;
      break;
    case 2:
      pinmask = TEMP2_PINMASK;
      break;
    case 3:
      pinmask = TEMP3_PINMASK;
      break;
    }
  onewire_reset(pinmask);

  onewire_write_byte(CMD_SKIP_ROM, pinmask);
  onewire_write_byte(CMD_READ_SCRATCHPAD, pinmask);

  for (i=0; i<9; i++)
    {
      ow_buf[i] = onewire_read_byte(pinmask);
    }
   if (ow_buf[8] == onewire_crc_check(ow_buf, 9) && ow_buf[7] == 0x10)
     {
     temperatures[id] = ow_buf[0] | (ow_buf[1] << 8);
     temperatures[id] *= 8;
     temperatures[id] &= 0xFFF0;
     temperatures[id] += 12;
     temperatures[id] -= ow_buf[6];
     }
}


void issue_convert(unsigned char pinmask)
{
  onewire_reset(pinmask);
  onewire_write_byte(CMD_SKIP_ROM, pinmask);
  onewire_write_byte(CMD_CONVERT_T, pinmask);
}

// Keep conversions going on for each sensor attached
void operate_onewire(void)
{
  if(!conv_1_active)
    {
      read_DS18S20(1);
      issue_convert(TEMP1_PINMASK);
      reset_timeout(TEMP1_TIMEOUT);
      conv_1_active = TRUE;
    } else {
      conv_1_active = !timeout_occured(TEMP1_TIMEOUT, DS18S20_CONV_TIME);
    }

  if(!conv_2_active)
    {
      read_DS18S20(2);
      issue_convert(TEMP2_PINMASK);
      reset_timeout(TEMP2_TIMEOUT);
      conv_2_active = TRUE;
    } else {
      conv_2_active = !timeout_occured(TEMP2_TIMEOUT, DS18S20_CONV_TIME);
    }

  if(!conv_3_active)
    {
      read_DS18S20(3);
      issue_convert(TEMP3_PINMASK);
      reset_timeout(TEMP3_TIMEOUT);
      conv_3_active = TRUE;
    } else {
      conv_3_active = !timeout_occured(TEMP3_TIMEOUT, DS18S20_CONV_TIME);
    }
}

/*
 * To send a PING:
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,07,01,01,36,05,37,cf}
 *
 * To identify a Register 4 (expected response as per the above identification data):
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,08,01,01,36,02,04,2c,d8}
 *
 */
void operate_device(void)
{

  unsigned int nr_calls=0;
  unsigned char timeout, response_opcode, p;

  timeout = get_messaging_timeout();
  reset_timeout(RESPONSE_TIMEOUT);

  for(;;)
    {
      if (nr_calls++ >= 490)
      {
        nr_calls = 0;
        timeout_occured(RESPONSE_TIMEOUT, timeout);
        reset_timeout(RESPONSE_TIMEOUT);
      }

      operate_onewire();

      if (get_device_message() && !process_generic_messages())
        {
          switch (message_buffer.content[OPCODE])
            {
            case SET_REGISTER:
              p = 0x01 << message_buffer.content[PARAMETER_START]-1;
              if (message_buffer.content[PARAMETER_START+1]) P1 |= p; else P1 &= ~p;
              response_opcode = COMMAND_SUCCESS;
              break;
            case READ_REGISTER:
              // Switch register number
              if((p = message_buffer.content[PARAMETER_START] < 4))
                {
                  message_buffer.content[PARAMETER_START] = temperatures[p-1] & 0x00ff;
                  message_buffer.content[PARAMETER_START+1] = (temperatures[p-1] >> 8) & 0x00ff;
                  message_buffer.index = PARAMETER_START+1;
                  response_opcode = COMMAND_SUCCESS;
                } else {
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

void main(void)
{
  // Enable interrupts
  EA = 1;

  // Set 4800 baud @ 11.0592 MHz Crystal
  init_device_comm(HOST_ID,COMM_SPEED_4800_H);

  // onewire_test();
  conv_1_active = conv_2_active = conv_3_active = FALSE;

  reset_timeout(TEMP1_TIMEOUT);
  reset_timeout(TEMP2_TIMEOUT);
  reset_timeout(TEMP3_TIMEOUT);

  operate_device();

}
