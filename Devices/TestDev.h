/*
 * TestDev.h
 *
 *  Created on: Oct 1, 2022
 *      Author: doma
 */

#ifndef DEVICES_TESTDEV_H_
#define DEVICES_TESTDEV_H_


#define PIN_NCS P1_1
#define PIN_SDI_SDO P1_2
#define PIN_SCK P3_7
#define NCS_INACTIVE 1
#define NCS_ACTIVE 0
#define PIN_ACTIVE P1_0


// Reset/initialize the SPI bus
void reset_rheostats(void);

// Write 9-bit wiper data into the SPI device
// return success
bool write_wiper(unsigned int value, bool is_volatile);

// Read and return the value of the wiper memory of
// the SPI device
bool read_wiper(unsigned int *value, bool is_volatile);

// Write the provided value to the TCON register
bool write_tcon(unsigned char tcon_value);

/*
 * Private functions
 */

// Write a 16 bit command and data
static bool writecommand16(unsigned char command_byte, unsigned char data_byte);

// Read 8 or 9 bit data. Result holds the top bit (9th bit) of the data
static bool read16bits(unsigned int *result, unsigned char command);

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
static void sck_delay(void);

#endif /* DEVICES_TESTDEV_H_ */
