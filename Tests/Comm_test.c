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

test -> content[0]=0xa1;
test -> content[1]=0xa2;
test -> content[2]=0xa3;
test -> content[3]=0xa4;
test -> content[4]=0xa5;
test -> content[5]=0xa6;
test ->index=5;

send_response(23);




for(;;)
  {
    if ( (message_struct = get_message()) )
      {
        a++;
      }
    a--;
  }

}
