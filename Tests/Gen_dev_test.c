/*
 * Comm_test.c
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#include "Slave_comm.h"

#define HOST_ID 1

static struct message_struct *MSG_buffer;


void reverse_MSG_buffer(void)
{
  unsigned char i, j=MSG_buffer -> index, tmp;
  for(i=PARAMETER_START;i<=MSG_buffer -> index;i++)
    {
      tmp = MSG_buffer -> content[i];
      MSG_buffer -> content[i] = MSG_buffer -> content[j];
      MSG_buffer -> content[j] = tmp;
    }
}

/*
 * To send a PING:
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,07,01,01,36,05,37,cf}
 *
 * To identify a Register 4 (expected response "P1_4"):
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,08,01,01,36,02,04,2c,d8}
 *
 */
void data_communication_test(void)
{

  unsigned char p,response_opcode;
  unsigned int nr_calls=0, timeout;

  timeout = get_messaging_timeout();
  reset_timeout(RESPONSE_TIMEOUT);
  for(;;)
    {
      if (nr_calls++ >= 380)
      {
        nr_calls = 0;
        timeout_occured(RESPONSE_TIMEOUT, timeout);
        reset_timeout(RESPONSE_TIMEOUT);
      }

      if (get_device_message() != NULL )
        {
          switch (MSG_buffer->content[OPCODE])
          {
          case SET_REGISTER:
            p = 0x01 << MSG_buffer->content[PARAMETER_START]-1;
            if (MSG_buffer->content[PARAMETER_START+1]) P1 |= p; else P1 &= ~p;
            response_opcode = COMMAND_SUCCESS;
            break;
          case READ_REGISTER:
            p = 0x01 << MSG_buffer->content[PARAMETER_START]-1;
            MSG_buffer->content[PARAMETER_START] = (P1 & p);
            MSG_buffer->index=PARAMETER_START;
            response_opcode = COMMAND_SUCCESS;
            break;
          case IDENTTIFY_REGISTER:
            MSG_buffer->content[PARAMETER_START+3] = MSG_buffer->content[PARAMETER_START]+'0';
            MSG_buffer->content[PARAMETER_START] = 'P';
            MSG_buffer->content[PARAMETER_START+1] = '1';
            MSG_buffer->content[PARAMETER_START+2] = '_';
            MSG_buffer->index = PARAMETER_START+3;
            response_opcode = COMMAND_SUCCESS;
            break;
          case RESET_DEVICE:
            response_opcode = COMMAND_SUCCESS;
            break;
          case COMM_TEST_REVERSE_MESSAGE:
            reverse_MSG_buffer();
            response_opcode = COMMAND_SUCCESS;
            break;
          case PING:
            response_opcode = ECHO;
            break;
          case SET_COMM_SPEED:
            set_comm_speed(MSG_buffer->content[PARAMETER_START]);
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

  MSG_buffer = get_message_buffer();

  data_communication_test();

}
