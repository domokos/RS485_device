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

// The timer ISR routine prototype to be included in the main file
ISR(TIMER0,0);

// Base routine headers
void delay_100us(unsigned int _100usec_time);

// Wait for milliseconds
void delay_msec(unsigned int msec);

// Wait for seconds
void delay_sec(unsigned int sec);

#endif /* BASE_H_ */
