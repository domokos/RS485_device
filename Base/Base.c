/*
 * Base.c
 *
 *  Created on: Oct 28, 2012
 *      Author: doma
 *      Basic common routines for the RS485 device
 */

#include <at89x051.h>
#include "Base.h"

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
  TMOD = (TMOD&0xF0)|0x02;    // Set Timer 0 Autoreload mode
  TR0  = 1;       // Start Timer 0

  ET0  = 1; EA = 1; // Enable Timer0 interrupt
  while ( DELAY > 0 )
    {
      __asm nop __endasm;
    }             // Wait delaytime
  ET0  = 0;       // Disable Timer0 interrupt
  TR0  = 0;       // Stop Timer 0
  return;
}
