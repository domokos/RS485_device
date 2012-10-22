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

// Global delaytime
volatile unsigned int  DELAY;
// The delay timer ISR
void Timer0_ISR(void) __interrupt 1 __using 0
{
  if ( DELAY > 0 ) DELAY--;    // Timebase ticks for delay-routine
  return;
}
// The delay routine wait
void delay(unsigned int _100usec_time)
{
  if ( _100usec_time == 0 ) return;   // Return if delaytime is set to zero

/*      Crystal: 11.0592 MHz
 *      Freq. of 1 machine cycle (1 count) = 12 crystal pulse: 11059200/12 = 921600
 *      Length of 1 machine cycle: 1/921600 sec
 *      Clock ticks for 0.000100 = 100 usec (): 0.0001/(1/921600) = 92.16 ticks
 *      We will use 92 ticks - Reload value of 255-91=164 0xa4
 */

  DELAY = _100usec_time;
  TL0  = 0xa4;    // Start from 0xa4
  TH0  = 0xa4;    // Reload to 0xa4
  TMOD = (TMOD&0xF0)|0x02;    // Autoreload for Timer 0
  TR0  = 1;       // Start Timer 0

  ET0  = 1;       // Enable Timer0 interrupt
  while ( DELAY > 0 )
    {
      __asm nop __endasm;
    }             // Wait delaytime
  ET0  = 0;       // Disable Timer0 interrupt
  TR0  = 0;       // Stop Timer 0
  return;
}


void init_comm()
{
  // Setup the serial port operation mode
  // Reset receive and transmit interrupt flags and disable receiver enable
  RI  = 0;TI  = 0;REN = 1;
  // 8-bit UART mode for serial
  SM2 = 0;SM1 = 1;SM0 = 0;

  SMOD = 0;

  // Setup the serial port timer Timer1
  TL1  = 0xfa;    // Start from 250
  TH1  = 0xfa;    // Reload 250 4800 baud @ 11.0592 MHz Crystal
  TMOD = (TMOD&0x0f)|0x20;    // Autoreload for Timer 1 hellpers
  TR1  = 1;       // Start Timer 1


  //Clear receive_buffer
  comm_buffer.content[0] = MESSAGE_TERMINATOR;
  comm_buffer.index = 0;
  serial_char_needs_processing=0;

  state=AWAITING_COMMAND; // Set the initial state
  display_mode = DISPLAY_MODE_BASE; // Set the initial displaymode

  ES   = 1;     // Enable Serial interrupt

  return;
}

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

