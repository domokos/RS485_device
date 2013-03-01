/*
 * Onewire.h
 *
 *  Created on: Feb 27, 2013
 *      Author: doma
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "Base.h"

/*
 * Public module functions
 */

// Must be called periodically to do onewire housekeeing and keep
// temp sensor's udated
void operate_onewire(void);

// Do a 1-wire reset cycle
// return 1 if presense pulse detected, 0 if no device(s) present
char onewire_reset(unsigned char pinmask);

// Write a byte to the onewire bus
void onewire_write_byte(unsigned char b, unsigned char pinmask);

// Read a byte from the onewire bus
unsigned char onewire_read_byte(unsigned char pinmask);

// Check the crc of the onewire bus message
unsigned char onewire_crc_check(unsigned char *p, unsigned char num);

// Wait for 60us - used for 1wire timing
void delay_60us(void);

// Wait for 480us - used for 1wire timing
void delay_480us(void);

/*
 * Internal utility functions
 */

// Write a bit to the onewire bus
static void onewire_write_bit(unsigned int bit0_pinmask);

// Read a bit from the onewire bus
static unsigned char onewire_read_bit(unsigned char pinmask);

#endif /* ONEWIRE_H_ */
