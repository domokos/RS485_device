/*
 * Onewire.h
 *
 *  Created on: Feb 27, 2013
 *      Author: doma
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "Base.h"

char onewire_reset(void);
void onewire_write_byte(unsigned char b);
unsigned char onewire_read_byte(void);
unsigned char onewire_crc_check(unsigned char *p, unsigned char num);

void onewire_write_bit(unsigned char bit0);
unsigned char onewire_read_bit(void);
void delay_60us(void);
void delay_480us(void);


#endif /* ONEWIRE_H_ */
