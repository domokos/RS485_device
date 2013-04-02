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
 * Onewire commands
 */

#define CMD_READ_ROM            0x33
#define CMD_SKIP_ROM            0xCC
#define CMD_MATCH_ROM           0x55
#define CMD_CONVERT_T           0x44
#define CMD_READ_SCRATCHPAD     0xBE
#define CMD_WRITE_SCRATCHPAD    0x4E

/*
 * Onewire specific timing constatnts
 */

#define DS18x20_CONV_TIME 750 // Conversion time for 12 bits precision

/*
 * Onewire buffer
 */

extern unsigned char ow_buf[9];

/*
 * Public module functions
 */

// Do a 1-wire reset cycle and
// return if presense pulse detected
bool onewire_reset(unsigned char pinmask);

// Write a byte to the onewire bus
void onewire_write_byte(unsigned char b, unsigned char pinmask);

// Read a byte from the onewire bus
unsigned char onewire_read_byte(unsigned char pinmask);

// Check the crc of the onewire bus message
unsigned char calculate_onewire_crc(unsigned char *p, unsigned char num);

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
