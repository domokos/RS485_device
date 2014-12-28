#include "Extender.h"

/*
 * EXTENDER Switches specific function section
 *
 */

unsigned char extender_sw_outputs[NR_OF_SW_EXTENDERS];

// Reset the extender switches
void
reset_extender_switches(void)
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
}

// Set the output to the values passed
void
write_extender_switches(void)
{
  unsigned char i, j, mask;

  j = NR_OF_SW_EXTENDERS;

  // Reset the shift registers
  NMR_PIN = 0;
  NMR_PIN = 1;

  // Loop through all the extender switch values
  while (j--)
    {
      i = extender_sw_outputs[j];
      mask = 0x80;
      while (mask)
        {
          DS_PIN = (i & mask) > 0;
          SHCP_PIN = 1;
          SHCP_PIN = 0;

          mask >>= 1;
        }
    }
  STCP_PIN = 1;
  STCP_PIN = 0;
}

// Return the value of the extender register referenced
// 0 is the first and 7 is the last in an 8 SW register
__bit
get_extender_switch(unsigned char reg_nr)
{
  unsigned char mask;

  mask = 0x01 << (reg_nr % 8);

  return (extender_sw_outputs[reg_nr / 8] & mask) > 0;
}

// Set the extender buffer value
// 0 is the first and 7 is the last in an 8 SW register
void
set_extender_switch_buffer(unsigned char reg_nr, __bit value)
{
  unsigned char mask;

  mask = 0x01 << (reg_nr % 8);

  if (value)
    extender_sw_outputs[reg_nr / 8] |= mask;
  else
    extender_sw_outputs[reg_nr / 8] &= ~mask;
}
