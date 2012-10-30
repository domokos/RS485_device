/*
 * Comm_test.c
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#include "Comm.h"

struct message_struct *message_struct;

void main()
{
unsigned short a=0;

for(;;)
  {
    if ( (message_struct = get_message() ) != 0x0 )
      {
        a++;
      }
    a--;
  }

}
