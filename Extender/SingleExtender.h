/*
 * Extender.h
 *
 *  Created on: Dec 27, 2014
 *      Author: dmolnar
 */

#ifndef SINGLEEXTENDER_H_
#define SINGLEEXTENDER_H_

#include "Base.h"


/*
 * A 74HC595, 8-bit parallell or serial out shift register is used az an extender switch .
 */

void reset_extender(void);

void write_extender(unsigned char reg_nr, bool value);

__bit get_extender_value(unsigned char reg_nr);

#endif /* SINGLEEXTENDER_H_ */
