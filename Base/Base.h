/*
 * Base.h
 *
 *  Created on: Oct 28, 2012
 *      Author: doma
 */

#ifndef BASE_H_
#define BASE_H_

#include <stddef.h>
#include "at89c4051.h"

// Global constants
#define TRUE 1
#define FALSE 0

typedef __bit bool;

#define ONE_MS_TIMEOUT 1

/*
 * Internal utility functions
 */

// The timer ISR routine prototype to be included in the main file
ISR(TIMER0,0);

// Initiaize the timer
static void init_timer(void);


/*
 * Public functions
 */

// Wait for seconds
void delay_sec(unsigned int sec);

// Wait for milliseconds
void delay_msec(unsigned int msec);

// start and reset the messaging timeout counter
void reset_timeout();

// Return if there was a timeout
// The calling parameter holds the timeout limit in miliseconds
unsigned char timeout_occured(unsigned int timeout_limit);

#endif /* BASE_H_ */
