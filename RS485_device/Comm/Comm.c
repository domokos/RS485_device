/*
 * Comm.c
 *
 *  Created on: Oct 20, 2012
 *      Author: Molnár Domokos
 *
 *      This is a communication module for the microprocessor over
 *      RS485
 */

#include <at89x051.h>
#include "Comm.h"

// Global variables facilitating communication
volatile unsigned char serial_received_char;
volatile __bit serial_char_needs_processing;
volatile __bit serial_send_complete;


// The serial ISR for communication
void Serial_ISR(void)  __interrupt 4 __using 0
{
if(RI==1)
  {
    RI=0;
    serial_received_char=SBUF;
    serial_char_needs_processing = 1;
  }else if(TI==1){
    TI=0;
    serial_send_complete = 1;
  }
return;
}
