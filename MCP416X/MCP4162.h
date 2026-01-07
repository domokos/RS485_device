/*
 * MCP4162.h
 *
 *  Created on: Nov 19, 2014
 *      Author: dmolnar
 *
 *      This module handles read/write of the wiper memory of the MCP4162
 *      Rheostat
 */

#ifndef MCP4162_H_
#define MCP4162_H_

#include "MCP416x.h"

// Constants defining if a read/write should address
// the volatile or the non-volatile wiper memory
#define VOLATILE 0
#define NON_VOLATILE 1

// State of the nCS line
#define NCS_INACTIVE 1
#define NCS_ACTIVE 0

#define WIPER_HEAT 1
#define WIPER_HW 0

/*
 * Public functions
 */

// Reset/initialize the SPI bus
void reset_rheostats(void);

// Write 9-bit wiper data into the SPI device
// return success
bool write_wiper(unsigned int value, bool is_volatile, __bit wiper_selector);

// Set the TCON register
bool set_tcon(unsigned char value, __bit wiper_selector);

// Read and return the value of the wiper memory of
// the SPI device
bool read_wiper(unsigned int *value, bool is_volatile, __bit wiper_selector);

/*
 * Private functions
 */

// Write 16 bits to the wiper
static bool
write16bit(unsigned char command_byte, unsigned char data_byte, __bit wiper_selector);

// Helper to write bits
static void
write_SPI_bits(unsigned char data, unsigned char bit_count);

// Helper to read bits
static unsigned char
read_SPI_bits(unsigned char bit_count);

#endif /* MCP4161_H_ */
