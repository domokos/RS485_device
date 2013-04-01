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

// Speed of the crystal
//#define CRYSTAL_SPEED_LO // 11.0592MHz
#define CRYSTAL_SPEED_HI // 22.1184MHz

typedef __bit bool;

#define ONE_MS_TIMEOUT 1
#define ONE_SEC_TIMEOUT 1000

// Timeout types
#define MSG_TIMEOUT 0
#define RESPONSE_TIMEOUT 1
#define DELAY_TIMEOUT 2
#define TEMP_CONV_TIMER 3
#define PWM1_TIMER 4

// Watchdog pin
#define WATCHDOG_PIN P3_5

/*
 * Internal utility functions
 */

// The timer ISR routine prototype to be included in the main file
ISR(TIMER0,0);

/*
 * Public functions
 */

// Initiaize the timer
void init_timer(void);


// Wait for seconds
void delay_sec(unsigned int sec);

// Wait for milliseconds
void delay_msec(unsigned int msec);

// start and reset the messaging timeout counter
void reset_timeout(unsigned char type);

// Get the time elapsed since reset
unsigned int get_time_elapsed(unsigned char type);

// Return if there was a timeout
// The calling parameter holds the timeout limit in miliseconds
bool timeout_occured(unsigned char type, unsigned int timeout_limit);

#endif /* BASE_H_ */
