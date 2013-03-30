/*
 * Genric_device.c
 *
 *  Created on: Mar 1, 2013
 *      Author: dmolnar
 */

#include "Generic_device.h"

bool process_generic_messages()
{
  unsigned char response_opcode=RESPONSE_UNDEFINED, p, i;
  bool message_handled = TRUE;

  switch (message_buffer.content[OPCODE])
  {
  case IDENTTIFY_REGISTER:
    i = message_buffer.content[PARAMETER_START];
    if (nr_of_registers <= i)
      {
      for(p=0; p< REG_IDENTIFICATION_LEN; p++)
        {
          message_buffer.content[PARAMETER_START+p] = register_identification[i][p];
        }
      message_buffer.index = PARAMETER_START+4;
      response_opcode = COMMAND_SUCCESS;
      } else {
      response_opcode = COMMAND_FAIL;
      }
    break;
  case RESET_DEVICE:
    response_opcode = COMMAND_SUCCESS;
    break;
  case PING_DEVICE:
    response_opcode = DEVICE_ECHO;
    break;
  case SET_COMM_SPEED:
    set_comm_speed(message_buffer.content[PARAMETER_START]);
    response_opcode = COMMAND_SUCCESS;
    break;
  case GET_DEVICE_CRC_ERROR_COUNTER:
    message_buffer.content[PARAMETER_START] = get_CRC_error_count();
    message_buffer.index = PARAMETER_START;
    response_opcode = COMMAND_SUCCESS;
    break;
  default:
    message_handled = FALSE;
   break;
  }

  if(message_handled) send_response(response_opcode);

  return message_handled;
}
