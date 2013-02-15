/*
 * Comm_test.c
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#include "Comm.h"

#define HOST_ID 1

struct message_struct *message_buffer;

void initial_test(void)
{
  struct message_struct *test;

  unsigned char a=0;
  test = get_message_buffer();

  test -> content[0]='A';
  test -> content[1]='l';
  test -> content[2]='m';
  test -> content[3]='a';
  test -> content[4]='f';
  test -> content[5]='a';
  test ->index=5;

  send_response('m','l');


  /*
   To test send this valid message in the emulator:
  {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,01,4b,75,74,79,61,ff,b6,10}
  Original: 014b75747961
  Flipped: 80d2ae2e9e86
  80d2ae2e9e86
  CRC flipped: b610
  */

  for(;;)
    {
      if ( (message_buffer = get_message()) != NULL )
        {
          message_buffer -> content[0]='S';
          message_buffer -> content[1]='i';
          message_buffer -> content[2]='k';
          message_buffer -> content[3]='e';
          message_buffer -> content[4]='r';
          message_buffer -> index=4;
          send_response('k','i');
        }
      a--;
    }
}

void reverse_message_buffer(struct message_struct *_message_buffer)
{
  unsigned char i, j=_message_buffer -> index, tmp;
  for(i=PARAMETER_START;i<=_message_buffer -> index;i++)
    {
      tmp = _message_buffer -> content[i];
      _message_buffer -> content[i] = _message_buffer -> content[j];
      _message_buffer -> content[j] = tmp;
    }
}


void flood_test(void)
{
  while (1)
    {
      bus_flood_test(SBUF, 500);
    }

}

/*
 * To send a PING:
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,01,36,05,ff,0c,6b}
 *
 * To identyfy a Register 4 (expected response "P1_4"):
 * {ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,01,36,02,04,ff,9e,5c,ff}
 *
 */
void data_communication_test(void)
{
  unsigned char p,response_opcode;
  for(;;)
    {
      if ( (message_buffer = get_message()) != NULL )
        {
          switch (message_buffer->content[OPCODE])
          {
          case SET_REGISTER:
            p = 0x01 << message_buffer->content[PARAMETER_START]-1;
            if (message_buffer->content[PARAMETER_START+1]) P1 |= p; else P1 &= ~p;
            response_opcode = COMMAND_SUCCESS;
            break;
          case READ_REGISTER:
            p = 0x01 << message_buffer->content[PARAMETER_START]-1;
            message_buffer->content[PARAMETER_START] = (P1 & p);
            message_buffer->index=PARAMETER_START;
            response_opcode = COMMAND_SUCCESS;
            break;
          case IDENTTIFY_REGISTER:
            message_buffer->content[PARAMETER_START+3] = message_buffer->content[PARAMETER_START]+'0';
            message_buffer->content[PARAMETER_START] = 'P';
            message_buffer->content[PARAMETER_START+1] = '1';
            message_buffer->content[PARAMETER_START+2] = '_';
            message_buffer->index = PARAMETER_START+3;
            response_opcode = COMMAND_SUCCESS;
            break;
          case RESET_DEVICE:
            response_opcode = COMMAND_SUCCESS;
            break;
          case COMM_TEST_REVERSE_MESSAGE:
            reverse_message_buffer(message_buffer);
            response_opcode = COMMAND_SUCCESS;
            break;
          case PING:
            response_opcode = ECHO;
            break;
          case SET_COMM_SPEED:
            set_comm_speed(message_buffer->content[PARAMETER_START]);
            response_opcode = COMMAND_SUCCESS;
            break;
          default:
            response_opcode = COMMAND_FAIL;
           break;
          }
          send_response(response_opcode,message_buffer->content[SEQ]);
        }
      p = get_comm_state();
      if (p == 1) P1_1 = 1;
      if (p == 2) P1_2 = 1;
      if (p == 3) P1_3 = 1;
      if (p == 4) P1_4 = 1;
      if (p == 5) P1_5 = 1;
    }
}




void main(void)
{

  P1=0;
  // Set 4800 baud @ 11.0592 MHz Crystal
  init_comm(HOST_ID,COMM_SPEED_4800_H);


//  initial_test();

  data_communication_test();
  // flood_test();

//  while(1)
//  {
//	  delay_msec(100);
//	  P1_4 = !P1_4;
//  }
}
