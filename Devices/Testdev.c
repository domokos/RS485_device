/*
 * Testdev.c
 *
 *  Created on: Oct 1, 2022
 *      Author: doma
 */

#include "Base.h"
#include "TestDev.h"


void reset_rheostats(void)
{
// Deselect chip
  PIN_NCS = NCS_INACTIVE;

// Prepare for SPI Mode 0,0
  PIN_SCK = 0;
}

bool
write_wiper(unsigned int value, bool is_volatile)
{
  unsigned char command_byte, data_byte;

// Set command and addres part of the command byte
  if(is_volatile)
      command_byte = 0x00;
    else
      command_byte = 0x20;

// Set the upper part of data to the
// lowest two bits of the command byte
  command_byte |= (unsigned char) (value  >> 8);
  data_byte = (unsigned char) (value & 0x00ff);
  data_byte = 0xaa;
  return writecommand16(command_byte, data_byte);
}

bool
read_tcon_and_status(unsigned int *value)
{
  __bit success = 1;
  unsigned char tmptcon;

  // Read tcon
  success &= read16bits(value, 0x4c);

  tmptcon = *value & 0x00ff;

  // Read status
  success &= read16bits(value, 0x5c);

  *value = *value << 8 | tmptcon;
  return success;
}

bool
read_wiper(unsigned int *value, bool is_volatile)
{
  if(is_volatile)
    return read16bits(value, 0x0c);
  else
    return read16bits(value, 0x2c);
}


bool
write_tcon(unsigned char tcon_value)
{
  // Command: Write TCON
  return writecommand16(0x40, tcon_value);
}

/*
 * Private functions of the module
 */

static bool
writecommand16(unsigned char command_byte, unsigned char data_byte)
{
   set_clock_lo();
  // Activate the chip
   PIN_NCS = NCS_ACTIVE;

 // Write the frist six bits to the SPI interface
   write_SPI_bits(command_byte, 6);

 //  Read CMDERR condition and reset the bus/return failure if error is detected
 //  Set pin to HI to read SDI/SDO line
   set_clock_lo();
   sck_delay();
   PIN_SDI_SDO = 1;
   sck_delay();

   set_clock_hi();
   sck_delay();
   sck_delay();
   if(PIN_SDI_SDO == 0)
     {
       reset_rheostats();
       return FALSE;
     }
   sck_delay();

 // Write the last bit of the command byte
   set_clock_lo();
   PIN_SDI_SDO = command_byte & 0x01;
   set_clock_hi();

 // Write the data_byte
//   data_byte = 0xaa;
   write_SPI_bits(data_byte, 8);

   PIN_SCK = 0;
   sck_delay();
   sck_delay();
   reset_rheostats();

   return TRUE;
}

static bool
read16bits(unsigned int *result, unsigned char command)
{
  unsigned char data_byte;

// Activate the chip
  PIN_NCS = NCS_ACTIVE;

// Write the six command bits to the SPI interface
  write_SPI_bits(command, 6);

//  Read CMDERR condition and reset the bus/return failure if error is detected
//  Set pin to HI to read SDI/SDO line
  set_clock_lo();
  PIN_SDI_SDO = 1;

  set_clock_hi();
  if(PIN_SDI_SDO == 0)
    {
      reset_rheostats();
      return FALSE;
    }
  set_clock_lo();
  set_clock_hi();
  // Read bit 8
  if (PIN_SDI_SDO)
    command |= 0x01;

  // Read the remaining 8 bits
  data_byte = read_SPI_bits(8);
  reset_rheostats();

  // Return the command and the data so caller can see the entire communication
  // Caller needs to mask data it needs
  *result = command << 8 | data_byte;
  return TRUE;
}

static void
set_clock_hi(void)
{
  sck_delay();
  PIN_SCK = 1;
  sck_delay();
}

static void
set_clock_lo(void)
{
  sck_delay();
  PIN_SCK = 0;
  sck_delay();
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

/*
 *  Wait for about 2.5 us - used for SPI clock timing
 *  we are targetting a 100 kHz clock frequency. With 4 calls to
 *  sck_delay per period we get a minimum of 2.5*4=10us period, which is a 100 kHz frequency.
 *  As per the datasheet the MCP4161 allows a maximum clock frequency of 250 kHz
 *  when reading non-volatile memory w/o external pullup.
 *  So setting 100 kHz should be safe for everything
 */
#ifdef  CRYSTAL_SPEED_LO

static void sck_delay(void)
{
        __asm
        mov     r2, #1
spi_clock_delay_loop:
        nop
        nop
        djnz    r2, spi_clock_delay_loop
        __endasm;
}

#elif defined CRYSTAL_SPEED_HI

static void sck_delay(void)
{
        __asm
        mov     r2, #1
spi_clock_delay_loop:
        djnz    r2, spi_clock_delay_loop
        __endasm;
}

#else
#error "No or incorrect crystal speed defined."
#endif




void
main(void)
{
// Enable interrupts and initialize timer
  EA = 1;
  init_timer();

  unsigned int i;

  PIN_ACTIVE = 0;
  for(i=1;i<240;i+=10)
    {
      write_wiper(i, TRUE);
      delay_sec(2);
      PIN_ACTIVE = !PIN_ACTIVE;
    }
  PIN_ACTIVE = 1;
  while(1)
    {
      __asm
      nop
      __endasm;
    }
}
