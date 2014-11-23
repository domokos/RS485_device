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

// SPI buffer
extern unsigned char spi_buf[2];

// Constants defining if a read/write should address
// the volatile or the non-volatile wiper memory
#define VOLATILE 0
#define NON_VOLATILE 1

// State of the nCS line
#define NCS_INACTIVE 1
#define NCS_ACTIVE 0

// Define the pins of the SPI interface
#define PIN_NCS P1_0
#define PIN_SCK P3_7
#define PIN_SDI_SDO P3_4

// Define the error condition return value
#define SPI_ERROR 0xfe00

/*
 * Public functions
 */

// Reset/initialize the SPI bus
void spi_reset(void);

// Write 9-bit wiper data into the SPI device
// return success
bool write_wiper(unsigned int value, bool is_volatile);

// Read and return the value of the wiper memory of
// the SPI device
unsigned int read_wiper(bool is_volatile);

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

// Wait for 60us - used for 1wire timing
static void
delay_60us(void);


#endif /* MCP4161_H_ */
