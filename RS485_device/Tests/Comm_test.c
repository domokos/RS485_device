/*
 * Comm_test.c
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#include "Comm.h"

void main()
{
unsigned short a=0;

for(;;)
  {
    if ( operate_comm() )
      {
        a = get_message()->index;
      }
    a++;
  }

}
