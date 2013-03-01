/*
 * Genric_device.c
 *
 *  Created on: Mar 1, 2013
 *      Author: dmolnar
 */

#include "Generic_device.h"


void reverse_MSG_buffer(void)
{
  unsigned char i, j=message_buffer.index, tmp;
  for(i=PARAMETER_START;i<=message_buffer.index;i++)
    {
      tmp = message_buffer.content[i];
      message_buffer.content[i] = message_buffer.content[j];
      message_buffer.content[j] = tmp;
    }
}

bool process_generic_messages()
{
  unsigned char response_opcode=0, p, i;
  bool message_handled = TRUE;

  switch (message_buffer.content[OPCODE])
  {
  case IDENTTIFY_REGISTER:
    i = message_buffer.content[PARAMETER_START]-1;
    for(p=0; p< REG_IDENTIFICATION_LEN; p++)
      {
        message_buffer.content[PARAMETER_START+p] = register_identification[i][p];
      }
    message_buffer.index = PARAMETER_START+4;
    response_opcode = COMMAND_SUCCESS;
    break;
  case RESET_DEVICE:
    response_opcode = COMMAND_SUCCESS;
    break;
  case COMM_TEST_REVERSE_MESSAGE:
    reverse_MSG_buffer();
    response_opcode = COMMAND_SUCCESS;
    break;
  case PING_DEVICE:
    response_opcode = DEVICE_ECHO;
    break;
  case SET_COMM_SPEED:
    set_comm_speed(message_buffer.content[PARAMETER_START]);
    response_opcode = COMMAND_SUCCESS;
    break;
  default:
    message_handled = FALSE;
   break;
  }

  if(message_handled) send_response(response_opcode);

  return message_handled;
}
