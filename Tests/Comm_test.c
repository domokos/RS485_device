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

// Set 19200 baud @ 11.0592 MHz Crystal
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
    if ( (message_struct = get_message()) )
      {
    	test -> content[0]=0x53;
    	test -> content[1]=0x49;
    	test -> content[2]=0x4b;
    	test -> content[3]=0x45;
    	test -> content[4]=0x52;
    	test ->index=4;
    	send_response(0x4b);
//Test CRC sent out
    	P0 = test -> content[5]; //Expected  d6
        P1 = test -> content[6]; //Expected  8b
    	test ->index=0;
      }
    a--;
  }

}
