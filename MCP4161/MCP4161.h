/*
 * MCP1646.h
 *
 *  Created on: Nov 19, 2014
 *      Author: dmolnar
 *
 *      This module handles read/write of the wiper memory of the MCP4161-502E/P
 */

#ifndef MCP4161_H_
#define MCP4161_H_

#include "Base.h"

// Constants defining if a read/write should address
// the volatile or the non-volatile wiper memory
#define VOLATILE 0
#define NON_VOLATILE 1

// State of the nCS line
#define NCS_INACTIVE 1
#define NCS_ACTIVE 0

/*
 * Public functions
 */

// Reset/initialize the SPI bus
void rheostat_reset(void);

// Write 9-bit wiper data into the SPI device
// return success
bool write_wiper(unsigned int value, bool is_volatile);

// Read and return the value of the wiper memory of
// the SPI device
bool read_wiper(unsigned int *value, bool is_volatile);

/*
 * Private functions
 */

// helper to set clock line with wait times
static void
set_clock_hi(void);

// helper to clear clock line with wait times
static void
set_clock_lo(void);

// Helper to write bits
static void
write_SPI_bits(unsigned char data, unsigned char bit_count);

// Helper to read bits
static unsigned char
read_SPI_bits(unsigned char bit_count);

// Precision wait for 60us
static void delay_60us(void);

#endif /* MCP4161_H_ */
