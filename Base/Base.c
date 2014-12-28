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

static unsigned int timer_start_times[5];

/*
 * Internal utility functions
 */


// The timer ISR - set up to occur every 1 ms for a 11.0592 MHz Crystal
ISR(TIMER0,0)
{
  // Increase timer
  time_counter++;

  // Keep watchdog happy
  WATCHDOG_PIN = !WATCHDOG_PIN;

  // Restart the timer
  TR0  = 0;

// use 17 machine cycles less to compensate for time
// spent executing the ISR itself

#ifdef  CRYSTAL_SPEED_LO
  TL0  = 0x50;   // Restart from 0xfc66 (1 ms) - decimal 1c = 0xfc50
  TH0  = 0xfc;
#elif defined CRYSTAL_SPEED_HI
  TL0  = 0xb6;   // Restart from 0xf8cc (1 ms) - decimal 1c = 0xf8b6
  TH0  = 0xf8;
#else
#error "No or incorrect crystal speed defined."
#endif
  TR0  = 1;
}


/*
 * Public functions
 */

// Initiaize the timer
void init_timer(void)
{
  if(timer_initialized) return;

  TR0  = 0;
#ifdef  CRYSTAL_SPEED_LO
  TL0  = 0x50;    // Start from 0xfc50
  TH0  = 0xfc;
#elif defined CRYSTAL_SPEED_HI
  TL0  = 0xb6;    // Start from 0xf8b6
  TH0  = 0xf8;
#else
#error "No or incorrect crystal speed defined."
#endif
  TMOD = (TMOD&0xF0)|0x01;    // Set Timer 0 16-bit mode
  TR0  = 1;       // Start Timer 0
  ET0  = 1;      // Enable Timer0 interrupt
  timer_initialized = TRUE;
}


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

  timer_start_times[type] = time_counter;
}

// Get the time elapsed since reset
unsigned int get_time_elapsed(unsigned char type)
{
  unsigned int counter;

  // Get the time counter value
  ET0  = 0;
  counter = time_counter;
  ET0  = 1;

  // If there is no owerflow in the interrupt ticks
  // (equality is regarded as no timeout - just started)
  if (counter >= timer_start_times[type])
    {
      return counter - timer_start_times[type];
  // There was an overflow - no multiple overflow is expected - timer must firs be reset and then queried regularily
    } else {
      return ((unsigned int)((unsigned int) 0xffff - timer_start_times[type])) + counter;
    }
}

// Return if there was a timeout
// The calling parameter holds the timeout limit in miliseconds
bool timeout_occured(unsigned char type, unsigned int timeout_limit)
{
 return get_time_elapsed(type) >= timeout_limit;
}
