#include "SingleExtender.h"

/*
 * EXTENDER Switches specific function section
 *
 */

static unsigned char extender_buffer = 0;

// Reset the extender ouptuts
void reset_extender(void)
{
  NOE_PIN = 1;
  DS_PIN = 0;
  SHCP_PIN = 0;
  NMR_PIN = 0;

  STCP_PIN = 0;
  STCP_PIN = 1;
  STCP_PIN = 0;

  NMR_PIN = 1;
  NOE_PIN = 0;

  extender_buffer = 0;
}

// Set the output to the values passed
void write_extender(unsigned char reg_nr, bool value)
{
  unsigned char mask;

  // Store the value in the internal buffer
  mask = 0x80 >> reg_nr;

  if (value)
    extender_buffer |= mask;
  else
    extender_buffer &= ~mask;

  // Reset the shift registers
  NMR_PIN = 0;
  NMR_PIN = 1;

  // Write the buffer to the extender
  // Loop through all bits and write shift register
  mask = 0x80;
  while (mask)
    {
      DS_PIN = (extender_buffer & mask) > 0;
      SHCP_PIN = 1;
      SHCP_PIN = 0;

      mask >>= 1;
    }

  // Gate the new shift register value to the outputs
  STCP_PIN = 1;
  STCP_PIN = 0;
}

// Return the value of the extender register referenced
// 0 is the first and 7 is the last in an 8 SW register
__bit get_extender_value(unsigned char reg_nr)
{
  unsigned char mask;

  mask = 0x80 >> reg_nr;

  return (extender_buffer & mask) > 0;
}
