/*
 * Base.c
 *
 *  Created on: Oct 28, 2012
 *      Author: doma
 *      Basic common routines for the RS485 device
 */
#include "Base.h"

// Global variables
bool timer_initialized = FALSE;
static volatile unsigned int  time_counter;
static unsigned int msg_timeout_start, response_timeout_start, delay_timeout_start;

/*
 * Internal utility functions
 */


// The timer ISR - set up to occur every 1 ms
ISR(TIMER0,0)
{
  time_counter++;
  TR0  = 0;

  // Restart from 0xfc66 (1 ms)
  TL0  = 0x66;
  TH0  = 0xfc;

  TR0  = 1;
}

// Initiaize the timer
static void init_timer(void)
{
  TL0  = 0x66;    // Start from 0xfc66
  TH0  = 0xfc;
  TMOD = (TMOD&0xF0)|0x01;    // Set Timer 0 16-bit mode
  TR0  = 1;       // Start Timer 0
  ET0  = 1;      // Enable Timer0 interrupt
  timer_initialized = TRUE;
}


/*
 * Public functions
 */


// Wait for seconds
void delay_sec(unsigned int sec)
{
  if ( sec == 0 ) return;   // Return if delaytime is zero

  reset_timeout(DELAY_TIMEOUT);

  while ( !timeout_occured(DELAY_TIMEOUT, ONE_SEC_TIMEOUT))
    {
      __asm nop __endasm;
    }             // Wait delaytime
  return;
}

// Wait for miliseconds
void delay_msec(unsigned int msec)
{
  if ( msec == 0 ) return;   // Return if delaytime is zero

/*      Crystal: 11.0592 MHz
 *      Freq. of 1 machine cycle (1 count) = 12 crystal pulse: 11059200/12 = 921600
 *      Length of 1 machine cycle: 1/921600 sec
 *      Clock ticks for 0.000100 = 100 usec (): 0.0001/(1/921600) = 92.16 ticks
 *      Clock ticks for 0.001 = 1 ms : 921.6 ticks
 *      We will use 922 ticks - to reload timer registers: FC66
 */

  reset_timeout(DELAY_TIMEOUT);

  while ( !timeout_occured(DELAY_TIMEOUT, ONE_MS_TIMEOUT))
    {
      __asm nop __endasm;
    }             // Wait delaytime
  return;
}


// start and reset the messaging timeout counter
void reset_timeout(unsigned char type)
{
  // Initialize timer if it is not initialized
  if(!timer_initialized) init_timer();

  if (type == MSG_TIMEOUT)
    {
      msg_timeout_start = time_counter;
    } else if (type == RESPONSE_TIMEOUT) {
      response_timeout_start = time_counter;
    // DELAY_TIMEOUT
    } else {
      delay_timeout_start = time_counter;
    }
}

// Return if there was a timeout
// The calling parameter holds the timeout limit in miliseconds
unsigned char timeout_occured(unsigned char type, unsigned int timeout_limit)
{
  unsigned int ticks_difference, timeout_start;

  // Set the start of the timeout based on timeout type
  if (type == MSG_TIMEOUT)
    {
      timeout_start = msg_timeout_start;
    } else if (type == RESPONSE_TIMEOUT) {
      timeout_start = response_timeout_start;
    // DELAY_TIMEOUT
    } else {
      timeout_start = delay_timeout_start;
    }

 // If there is no owerflow in the interrupt ticks
 // (equality as regarded as no timeout - just started)
 if (time_counter >= timeout_start)
   {
     ticks_difference = time_counter - timeout_start;
 // There was an overflow - no multiple overflow is expected - timer must firs be reset and then queried regularily
   } else {
     ticks_difference = ((unsigned int)((unsigned int) 0xffff - timeout_start)) + time_counter;
   }

 return ticks_difference >= timeout_limit;
}
