/*
 * Extender.h
 *
 *  Created on: Dec 27, 2014
 *      Author: dmolnar
 */

#ifndef EXTENDER_H_
#define EXTENDER_H_

#include "Base.h"


/*
 * For extender swich 74HC595, 8-bit parallell or serial out shift registers are used.
 * Extender switches need to be cascaded, meaning that the Q7S pin of registers need to be fed
 * into the DS of the next register, all other control lines should be connected.
 */

// This variable holds the state of extender switches
extern unsigned char extender_sw_outputs[NR_OF_SW_EXTENDERS];

void reset_extender_switches(void);

void write_extender_switches(void);

__bit get_extender_switch(unsigned char reg_nr);

void set_extender_switch_buffer(unsigned char reg_nr, __bit value);

#endif /* EXTENDER_H_ */
