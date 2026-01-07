/*
 * MCP4162.c
 *
 *  Created on: Nov 19, 2014
 *  Updated on: Nov 22, 2022 - Add HP controller
 *      Author: dmolnar
 *
 *  This module handles read/write of the wiper memory of the MCP4162-502E/P
 */

#include "MCP4162.h"

void reset_rheostats(void)
{
// Prepare for SPI Mode 0,0
  PIN_SCK = 0;
  PIN_SDO = 1;

// Cycle the CS pin
  PIN_NCS_CSEL = !PIN_NCS_CSEL;
}

bool
write_wiper(unsigned int value, bool is_volatile, __bit wiper_selector)
{
  unsigned char command_byte, data_byte;

// Set command and addres part of the command byte
  if(is_volatile)
      command_byte = 0x00;
    else
      command_byte = 0x20;

// Set the upper part of data to the
// lowest two bits of the command byte
  command_byte |= (unsigned char) (value >> 8);
  data_byte = (unsigned char) (value & 0xff);

  return write16bit(command_byte, data_byte, wiper_selector);
}

// Cannot be moved to common part as this calls a different static function
bool
set_tcon(unsigned char data_byte, __bit wiper_selector)
{
  unsigned char command_byte;
  command_byte = 0x40;

  return write16bit(command_byte, data_byte, wiper_selector);
}

bool
read_wiper(unsigned int *value, bool is_volatile, __bit wiper_selector)
{
  unsigned char command_byte, data_byte;

// Set command and addres part of the command byte
  if(is_volatile)
      command_byte = 0x0c;
    else
      command_byte = 0x2c;

// Select the appropriate chip
  PIN_NCS_CSEL = wiper_selector;

// Write the six command bits to the SPI interface
  write_SPI_bits(command_byte, 6);

//  Read CMDERR condition and reset the bus/return failure if error is detected
  set_clock_lo();
  set_clock_hi();
  if(PIN_SDO == 0)
    {
      reset_rheostats();
      return FALSE;
    }
  set_clock_lo();
  set_clock_hi();
  // Read bit 8
  if (PIN_SDO)
    command_byte |= 0x01;

  // Read the remaining 8 bits
  data_byte = read_SPI_bits(8);
  reset_rheostats();

  // Return the command and the data so caller can see the entire communication
  // Caller needs to mask data it needs
  *value = command_byte << 8 | data_byte;
  return TRUE;
}

/*
 * Private functions of the module
 */

static bool
write16bit(unsigned char command_byte, unsigned char data_byte, __bit wiper_selector)
{
// Select the appropriate chip
  PIN_NCS_CSEL = wiper_selector;

// Write the frist six bits to the SPI interface
  write_SPI_bits(command_byte, 6);

//  Read CMDERR condition and reset the bus/return failure if error is detected
  set_clock_lo();
  set_clock_hi();
  __asm
    nop
  __endasm;
  if(PIN_SDO == 0)
    {
      reset_rheostats();
      return FALSE;
    }
  set_clock_lo();

// Write the last bit of the command byte
  PIN_SERDATA = command_byte & 0x01;
  set_clock_hi();

// Write the data_byte
  write_SPI_bits(data_byte, 8);
  reset_rheostats();

  return TRUE;
}

static void
write_SPI_bits(unsigned char data, unsigned char bit_count)
{
  unsigned char i, mask;

  mask = 0x80;
  for(i=0;i<bit_count;i++)
    {
      set_clock_lo();
      PIN_SERDATA = (data & mask) > 0;
      set_clock_hi();
      mask = mask >> 1;
    }
}

static unsigned char
read_SPI_bits(unsigned char bit_count)
{
  unsigned char i, mask, retval;

  retval = 0;
  mask = 0x80;
  for(i=0;i<bit_count;i++)
    {
      set_clock_lo();
      if (PIN_SDO) retval |= mask;
      set_clock_hi();
      mask = mask >> 1;
    }
  return retval;
}


