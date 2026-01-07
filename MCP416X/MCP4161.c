/*
 * MCP4161.c
 *
 *  Created on: Nov 19, 2014
 *      Author: dmolnar
 *
 *  This module handles read/write of the wiper memory of the MCP4161-502E/P
 */

#include "MCP4161.h"

void reset_potentiometer(void)
{
// Deselect chip
  PIN_NCS_HW = NCS_INACTIVE;

  PIN_NCS_HEAT = NCS_INACTIVE;

// Prepare for SPI Mode 0,0
  PIN_SCK = 0;
}

bool write_wiper(unsigned int value, bool is_volatile, __bit wiper_selector)
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

// Activate the chip
  if (wiper_selector == WIPER_HEAT)
    PIN_NCS_HEAT = NCS_ACTIVE;
  else
    PIN_NCS_HW = NCS_ACTIVE;

// Write the six command bits to the SPI interface
  write_SPI_bits(command_byte, 6);

//  Read CMDERR condition and reset the bus/return failure if error is detected
//  Set pin to HI to read SDI/SDO line
  set_clock_lo();
  PIN_SDI_SDO = 1;

  set_clock_hi();
  if(PIN_SDI_SDO == 0)
    {
      reset_potentiometer();
      return FALSE;
    }
  set_clock_lo();
  set_clock_hi();
  // Read bit 8
  if (PIN_SDI_SDO)
    command_byte |= 0x01;

  // Read the remaining 8 bits
  data_byte = read_SPI_bits(8);
  reset_potentiometer();

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
// Activate the chip
  if (wiper_selector == WIPER_HEAT)
    PIN_NCS_HEAT = NCS_ACTIVE;
  else
    PIN_NCS_HW = NCS_ACTIVE;

// Write the frist six bits to the SPI interface
  write_SPI_bits(command_byte, 6);

//  Read CMDERR condition and reset the bus/return failure if error is detected
//  Set pin to HI to read SDI/SDO line
  set_clock_lo();
  PIN_SDI_SDO = 1;

  set_clock_hi();
  if(PIN_SDI_SDO == 0)
    {
      reset_potentiometer();
      return FALSE;
    }
  set_clock_lo();

// Write the last bit of the command byte
  PIN_SDI_SDO = command_byte & 0x01;
  set_clock_hi();

// Write the data_byte
  write_SPI_bits(data_byte, 8);
  reset_potentiometer();

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
      PIN_SDI_SDO = (data & mask) > 0;
      set_clock_hi();
      mask = mask >> 1;
    }
}

static unsigned char
read_SPI_bits(unsigned char bit_count)
{
  unsigned char i, mask, retval;

  // Set the data pin high to read the value set by the external chip
  PIN_SDI_SDO = 1;

  retval = 0;
  mask = 0x80;
  for(i=0;i<bit_count;i++)
    {
      set_clock_lo();
      if (PIN_SDI_SDO) retval |= mask;
      set_clock_hi();
      mask = mask >> 1;
    }
  return retval;
}


