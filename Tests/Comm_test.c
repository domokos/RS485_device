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
  {55,1,4b,75,74,79,61,b6,10,5d}
  Original: 14b75747961
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

void data_communication_test(void)
{
  if ( (message_buffer = get_message()) != NULL )
    {
      switch (message_buffer->content[OPCODE])
      {
        case PING:
            send_response(ECHO,message_buffer->content[SEQ]);
          break;
        case COMM_TEST_REVERSE_MESSAGE:

          break;
      }




    }
}


void main(void)
{

  // Set 14000 baud @ 11.0592 MHz Crystal
  init_comm(HOST_ID,COMM_SPEED_4800_H);


  initial_test();

  //data_communication_test();


}
