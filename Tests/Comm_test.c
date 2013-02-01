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

test -> content[0]='A';
test -> content[1]='l';
test -> content[2]='m';
test -> content[3]='a';
test -> content[4]='f';
test -> content[5]='a';
test ->index=5;

send_response('m','l');
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
    	test -> content[0]='S';
    	test -> content[1]='i';
    	test -> content[2]='k';
    	test -> content[3]='e';
    	test -> content[4]='r';
    	test ->index=4;
    	send_response('k','i');
//Test CRC sent out
    	P3 = test -> content[5]; //Expected  d6
        P1 = test -> content[6]; //Expected  8b
    	test ->index=0;
      }
    a--;
  }

}
