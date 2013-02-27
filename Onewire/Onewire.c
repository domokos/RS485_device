#include "Onewire.h"

#define ONEWIRE_PIN _P1_7

void onewire_write_byte(unsigned char b)
{
	unsigned char i=8;

	while(i)
	  {
            onewire_write_bit(b);
            b >>= 1;
            i--;
	  }
}

unsigned char onewire_read_byte(void)
{
	unsigned char i,b;
	b=0;i=8;
	while (i)
	  {
	    b = (b >> 1) | (onewire_read_bit() << 7);
	    i--;
	  }
	return b;
}

__code unsigned char onewire_crc_table[] = {
  0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
 35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
 70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
 17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
 50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
 87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
};


unsigned char onewire_crc_check(unsigned char *p, unsigned char num)
{
	unsigned char crc=0;

	while (num--) {
		crc = onewire_crc_table[crc ^ *p++];
	}
	return crc;
}

// disable "must return value", sdcc can't tell the asm
// code is actually returning a value
#pragma save
#pragma disable_warning 59

// Do a 1-wire reset cycle
// return 1 if presense pulse detected, 0 if no device(s) present
char onewire_reset(void)
{
	__asm
	clr	ONEWIRE_PIN	// do the reset pulse
	lcall	_delay_480us
	push	ie
	clr	ea		// disable interrupts
	setb	ONEWIRE_PIN
	lcall	_delay_60us	// want for the presence pulse
	mov	c, ONEWIRE_PIN	// read the presence pulse
	pop	ie		// restore interrupt status
	clr	a
	cpl	c
	rlc	a
	mov	dpl, a
	sjmp	_delay_480us	// wait the rest of init cycle
	__endasm;
}

void onewire_write_bit(unsigned char bit0)
{
	bit0;
	__asm
	mov	a, dpl
	push	ie
	clr	ea
	clr	ONEWIRE_PIN
	jnb	acc.0, onewire_write_bit0
onewire_write_bit1:
	setb	ONEWIRE_PIN
	pop	ie
	sjmp	_delay_60us
onewire_write_bit0:
	lcall	_delay_60us
	setb	ONEWIRE_PIN
	pop	ie
	__endasm;
}

unsigned char onewire_read_bit(void)
{
	__asm
	push	ie
	clr	ea
	clr	ONEWIRE_PIN
	mov	r2, #14
	setb	ONEWIRE_PIN
onewire_read_bit_wait:
	djnz	r2, onewire_read_bit_wait
	mov	c, ONEWIRE_PIN
	pop	ie
	lcall	_delay_60us
	clr	a
	rlc	a
	mov	dpl, a
	mov	r2, #40
onewire_read_cycle_wait:
	djnz	r2, onewire_read_cycle_wait
	__endasm;
}

#pragma restore



// CRYSTAL_SPEED_11.0592
// 921600 cycle/sec
// 1085.0694 ns
//
// 480 us = 442 cycles = 2*221
// 60 us = 55 cycles = 5*11
//
// CRYSTAL_SPEED_22.1184
// 1843200 cycle/sec
// 542.53 ns
//
// 480 us = 885 cycles = 5*177
// 60 us = 110.6 cycles = 2*52


#ifdef  CRYSTAL_SPEED_LO

// Wait for 60us - used for 1wire timing
void delay_60us(void)
{
        __asm
        mov     r2, #11
delay_60us_loop:
        nop
        nop
        nop
        djnz    r2, delay_60us_loop
        __endasm;
}

// Wait for 480us - used for 1wire timing
void delay_480us(void)
{
        __asm
        mov     r2, #221
delay_480us_loop:
        djnz    r2, delay_480us_loop
        __endasm;
}

#elif defined CRYSTAL_SPEED_HI

// Wait for 60us - used for 1wire timing
void delay_60us(void)
{
        __asm
        mov     r2, #52
delay_60us_loop:
        djnz    r2, delay_60us_loop
        __endasm;
}

// Wait for 480us - used for 1wire timing
void delay_480us(void)
{
        __asm
        mov     r2, #177
delay_480us_loop:
        nop
        nop
        nop
        djnz    r2, delay_480us_loop
        __endasm;
}
#endif
