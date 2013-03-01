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

__code const char register_identification[][REG_IDENTIFICATION_LEN] = {
   {REG_TYPE_TEMP, REG_RO, 9, 0, 0},
   {REG_TYPE_SW, REG_RW, 1, 0, 0},
   {REG_TYPE_TEMP, REG_RO, 9, 0, 0},
   {'A', 'l', 'm', 'a', 'k'}
};


/*
 * To send a PING:
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,07,01,01,36,05,37,cf}
 *
 * To identify a Register 4 (expected response as per the above identification data):
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,08,01,01,36,02,04,2c,d8}
 *
 */
void data_communication_test(void)
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
              p = 0x01 << message_buffer.content[PARAMETER_START]-1;
              message_buffer.content[PARAMETER_START] = (P1 & p);
              message_buffer.index=PARAMETER_START;
              response_opcode = COMMAND_SUCCESS;
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
  data_communication_test();

}
