/*
 * MCP416x.c
 *
 *  Created on: Jan 7, 2026
 *      Author: doma
 */

#include "MCP416x.h"

void set_clock_hi(void)
{
  sck_delay();
  PIN_SCK = 1;
  sck_delay();
}

void set_clock_lo(void)
{
  sck_delay();
  PIN_SCK = 0;
  sck_delay();
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

void sck_delay(void)
{
        __asm
	nop
	nop
	nop
	nop
        __endasm;
}

#elif defined CRYSTAL_SPEED_HI

void sck_delay(void)
{
        __asm
	nop
	nop
        __endasm;
}

#else
#error "No or incorrect crystal speed defined."
#endif
