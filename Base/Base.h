/*
 * Base.h
 *
 *  Created on: Oct 28, 2012
 *      Author: doma
 */

#ifndef BASE_H_
#define BASE_H_

#include<at89x051.h>

// Global constants
#define TRUE 1
#define FALSE 0

typedef __bit bool;

// Base routine headers
void delay(unsigned int _100usec_time);

#endif /* BASE_H_ */
