/*
 * Comm_test.c
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#include "Comm.h"

struct message_struct *message_struct, *test;

void main()
{
unsigned short a=0;

init_comm(1);

TH1 = 0xfd;

test = get_message_buffer();

test -> content[0]=0x74;
test -> content[1]=0x65;
test -> content[2]=0x78;
test -> content[3]=0x74;
test -> content[4]=0x69;
test -> content[5]=0x74;
test ->index=5;

send_response(0x78);
test ->index=0;


for(;;)
  {
    if ( (message_struct = get_message()) )
      {
        a++;
      }
    a--;
  }

}
