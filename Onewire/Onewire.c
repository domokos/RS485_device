#include "Onewire.h"

unsigned char ow_buf[9];

void onewire_write_byte(unsigned char b, unsigned char pinmask)
{
	unsigned char i=8;

	while(i)
	  {
            onewire_write_bit((unsigned int)(pinmask << 8) | b);
            b >>= 1;
            i--;
	  }
}

unsigned char onewire_read_byte(unsigned char pinmask)
{
	unsigned char i=8,b=0;

	while (i)
	  {
	    b = (b >> 1) | (onewire_read_bit(pinmask) << 7);
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


unsigned char calculate_onewire_crc(unsigned char *p, unsigned char num)
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


#ifdef ONEWIRE_IS_ON_P3
// Do a 1-wire reset cycle
// return true if presense pulse detected, 0 if no device(s) present
bool onewire_reset(unsigned char pinmask)
{
#ifdef SDCC
        pinmask;
#endif
	__asm
	mov     a, dpl
	mov     r7, a           // Store pinmask in r7
	cpl     a
	anl     _P3, a          // do the reset pulse
	                        // PORT &= ~pinmask - pull bus pin low
	lcall	_delay_480us
	push	ie
	clr	ea		// disable interrupts
	mov     a, r7
	orl     _P3, a          // PORT |= pinmask - pull bus pin high
	lcall	_delay_60us	// wait for the presence pulse
	mov     a, r7           // read the presence pulse
	anl	a, _P3
	pop	ie		// restore interrupt status
	clr     c
	jnz     no_presence_pulse
	setb    c
no_presence_pulse:
	lcall	_delay_480us	// wait the rest of init cycle
	__endasm;

#ifndef SDCC
	return FALSE;
#endif
}

void onewire_write_bit(unsigned int pinmask_bit0)
{
#ifdef SDCC
        pinmask_bit0;
#endif
	__asm
	mov	a, dpl
	mov     r6, a           // Store bit to be sent in r6
	mov     a, dph
	mov     r7, dph         // Store pinmask in r7
	push	ie
	clr	ea
	cpl     a
        anl     _P3, a          // PORT &= ~pinmask - pull bus pin low
        mov     a, r6
	jnb	acc.0, onewire_write_bit0
onewire_write_bit1:
        mov     a, r7
        orl     _P3, a          // PORT |= pinmask - pull bus pin high
	pop	ie
	sjmp	_delay_60us
onewire_write_bit0:
	lcall	_delay_60us
        mov     a, r7
        orl     _P3, a          // PORT |= pinmask  - pull bus pin high
	pop	ie
	__endasm;
}

unsigned char onewire_read_bit(unsigned char pinmask)
{
#ifdef SDCC
        pinmask;
#endif
	__asm
	mov     a, dph
	mov     r7, dph         // Store bitmask in r7
	push	ie
	clr	ea
	cpl     a
	anl     _P3, a          // PORT &= ~pinmask - pull bus pin low
#ifdef  CRYSTAL_SPEED_HI
	mov		r2, #2
#elif defined CRYSTAL_SPEED_LO
	mov     r2, #1
#else
#error "No or incorrect crystal speed defined."
#endif
	mov     a, r7
	orl     _P3, a          // PORT |= pinmask - pull bus pin high
onewire_read_bit_wait:
	djnz	r2, onewire_read_bit_wait
	mov     a, r7
	anl     a, _P3          // read the bus
	pop	ie
	mov	dpl, #0
	jz    	read_zero
	inc     dptr
read_zero:
	lcall	_delay_60us
	__endasm;

#ifndef SDCC
        return 0;
#endif
}


#elif defined ONEWIRE_IS_ON_P1

// Do a 1-wire reset cycle
// return true if presense pulse detected, 0 if no device(s) present
bool onewire_reset(unsigned char pinmask)
{
#ifdef SDCC
        pinmask;
#endif
        __asm
        mov     a, dpl
        mov     r7, a           // Store pinmask in r7
        cpl     a
        anl     _P1, a          // do the reset pulse
                                // PORT &= ~pinmask - pull bus pin low
        lcall   _delay_480us
        push    ie
        clr     ea              // disable interrupts
        mov     a, r7
        orl     _P1, a          // PORT |= pinmask - pull bus pin high
        lcall   _delay_60us     // wait for the presence pulse
        mov     a, r7           // read the presence pulse
        anl     a, _P1
        pop     ie              // restore interrupt status
        clr     c
        jnz     no_presence_pulse
        setb    c
no_presence_pulse:
        lcall   _delay_480us    // wait the rest of init cycle
        __endasm;

#ifndef SDCC
        return FALSE;
#endif
}

void onewire_write_bit(unsigned int pinmask_bit0)
{
#ifdef SDCC
        pinmask_bit0;
#endif
        __asm
        mov     a, dpl
        mov     r6, a           // Store bit to be sent in r6
        mov     a, dph
        mov     r7, dph         // Store pinmask in r7
        push    ie
        clr     ea
        cpl     a
        anl     _P1, a          // PORT &= ~pinmask - pull bus pin low
        mov     a, r6
        jnb     acc.0, onewire_write_bit0
onewire_write_bit1:
        mov     a, r7
        orl     _P1, a          // PORT |= pinmask - pull bus pin high
        pop     ie
        sjmp    _delay_60us
onewire_write_bit0:
        lcall   _delay_60us
        mov     a, r7
        orl     _P1, a          // PORT |= pinmask  - pull bus pin high
        pop     ie
        __endasm;
}

unsigned char onewire_read_bit(unsigned char pinmask)
{
#ifdef SDCC
        pinmask;
#endif
        __asm
        mov     a, dph
        mov     r7, dph         // Store bitmask in r7
        push    ie
        clr     ea
        cpl     a
        anl     _P1, a          // PORT &= ~pinmask - pull bus pin low
#ifdef  CRYSTAL_SPEED_HI
        mov             r2, #2
#elif defined CRYSTAL_SPEED_LO
        mov     r2, #1
#else
#error "No or incorrect crystal speed defined."
#endif
        mov     a, r7
        orl     _P1, a          // PORT |= pinmask - pull bus pin high
onewire_read_bit_wait:
        djnz    r2, onewire_read_bit_wait
        mov     a, r7
        anl     a, _P1          // read the bus
        pop     ie
        mov     dpl, #0
        jz      read_zero
        inc     dptr
read_zero:
        lcall   _delay_60us
        __endasm;

#ifndef SDCC
        return 0;
#endif
}

#else
#error "Onewire bus is not on any ports of the microcontroller"
#endif /* Onewire on which bus onewire is */

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

// Precision Wait for 60us - used for 1wire timing
static void delay_60us(void)
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

// Precision Wait for 480us - used for 1wire timing
static void delay_480us(void)
{
        __asm
        mov     r2, #221
delay_480us_loop:
        djnz    r2, delay_480us_loop
        __endasm;
}

#elif defined CRYSTAL_SPEED_HI

// Precision Wait for 60us - used for 1wire timing
static void delay_60us(void)
{
        __asm
        mov     r2, #52
delay_60us_loop:
        djnz    r2, delay_60us_loop
        __endasm;
}

// Precision Wait for 480us - used for 1wire timing
static void delay_480us(void)
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
#else
#error "No or incorrect crystal speed defined."
#endif

#ifdef ONEWIRE_READ_DS2405_REQUIRED

bool
ReadDS2405(unsigned char register_romcode[], unsigned char pinmask)
{
  unsigned char bit_index, byte_index, bit_pair_read;

  bit_index = 0;
  byte_index = 0;

  // SearchROM active only command
  onewire_write_byte(CMD_ACTIVE_ONLY_SEARCH, pinmask);

  do
    // for all eight bytes
    {
      bit_pair_read = 0;
      if (onewire_read_bit(pinmask) == 1)
        bit_pair_read = 2; // set bit 2 of the read pair
      if (onewire_read_bit(pinmask) == 1)
        bit_pair_read |= 1; // and set its complement


      if (bit_pair_read == 3) // There is no matching device on the 1-Wire - break and return false
        break;

      // Write the expected bit of the ROM code
      onewire_write_bit((unsigned int) ((pinmask << 8) | (register_romcode[byte_index] >> bit_index)));

      bit_index++; // increment bit_index

      if (bit_index == 8) // if the bit index is 8 then go to next ROM byte
      {
          byte_index++;
          bit_index = 0;
      }

    } while (byte_index < 8); //loop until through all ROM bytes 0-7 were matched

  // If search was successful then the device is active otherwise inactive
  return byte_index == 8;
}

#endif /* ONEWIRE_READ_DS2405_REQUIRED */


