/*
 * at894051.h
 *
 *  Created on: Jan 28, 2013
 *      Author: dmolnar
 */

#ifndef AT894051_H_
#define AT894051_H_


// Avoid false-alarm syntax errors in Eclipse.
#ifdef __CDT_PARSER__
#define __at(x)
#define __interrupt(x)
#define __using(x)
#define __code
#define __near
#endif

#define SFRBIT(address, name, bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0) \
  SFR(address, name)    \
  SBIT(address+0, bit0) \
  SBIT(address+1, bit1) \
  SBIT(address+2, bit2) \
  SBIT(address+3, bit3) \
  SBIT(address+4, bit4) \
  SBIT(address+5, bit5) \
  SBIT(address+6, bit6) \
  SBIT(address+7, bit7)

#ifdef SDCC
// Syntax for the SDCC (Small Device C Compiler).
#define SFR(address, name) static __sfr __at (address) name;
#define SBIT(address, name) static __sbit __at (address) name;
#define SFR16(addressH, addressL, name) static __sfr16 __at (((addressH) << 8) + (addressL)) name;
#define SFRX(address, name)       static volatile unsigned char __xdata __at(address) name;

/*! Defines or declares an interrupt service routine (ISR).
 * <b>For the interrupt to work, SDCC requires that the declaration must
 * be present in the file that defines main().</b>
 *
 * \param source
 *    The source of the interrupt.  Must be either the first word of one of
 *    the *_VECTOR macros defined in this file (e.g. "P1INT").
 *
 * \param bank
 *    The register bank to use.  Must be a number from 0 to 3, inclusive.
 *    If you choose a non-zero bank, then the compiler will assume that the
 *    ISR can modify the registers in that bank, and not bother to restore
 *    them to their original value.
 *    Therefore, we recommend choosing bank 0 unless you want to save some
 *    CPU time and you can guarantee that it is OK for the interrupt to
 *    modify those registers.
 *
 * Example ISR declaration (in a .h file):
\code
ISR(UTX1, 0);
\endcode
 *
 * Example ISR definition (in a .c file):
\code
ISR(UTX1, 0)
{
    // code for handling event and clearing interrupt flag
}
\endcode
 */
#define ISR(source, bank) void ISR_##source() __interrupt(source##_VECTOR) __using(bank)

#elif defined(__CDT_PARSER__)

// These definitions are here to avoid "Symbol x could not be resolved" errors
// from the Eclipse Code Analysis tool.
#define SFR(address, name) static unsigned char name;
#define SBIT(address, name) static unsigned char name;
#define SFR16(addressH, addressL, name) static unsigned short name;
#define SFRX(address, name) static unsigned char name;
#define __bit unsigned char

#else
#error "Unknown compiler."
#endif

/*  BYTE Register  */

SFR(0x81, SP)
SFR(0x82, DPL)
SFR(0x83, DPH)
SFR(0x87, PCON)

/*  TCON  */
SFRBIT(0x88, TCON, TF1, TR1, TF0, TR0, IE1, IT1, IE0, IT0)

SFR(0x89, TMOD)

SFR(0x8A,TL0)
SFR(0x8B,TL1)
SFR(0x8C,TH0)
SFR(0x8D,TH1)

/*  P1 */
SFRBIT(0x90, P1, P1_7, P1_6, P1_5, P1_4, P1_3, P1_2, P1_1, P1_0)

/*  SCON  */
SFRBIT(0x98, SCON, SM0, SM1, SM2, REN, TB8, RB8, TI, RI)

SFR(0x99,SBUF)
SFR(0xA8,IE)

/* P3 */
SFRBIT(0xB0, P3, P3_7, P3_6, P3_5, P3_4, P3_3, P3_2, P3_1, P3_0)

SFR(0xB8,IP)
SFR(0xD0,PSW)
SFR(0xE0,ACC)
SFR(0xF0,B)

/*  IE   */
SBIT(0xA8,EX0)
SBIT(0xA9,ET0)
SBIT(0xAA,EX1)
SBIT(0xAB,ET1)
SBIT(0xAC,ES)
SBIT(0xAF,EA)

SBIT(0xB0,RXD)
SBIT(0xB1,TXD)
SBIT(0xB2,INT0)
SBIT(0xB3,INT1)
SBIT(0xB4,T0)
SBIT(0xB5,T1)
SBIT(0xB6,WR)
SBIT(0xB7,RD)

/*  IP   */
SBIT(0xB8,PX0)
SBIT(0xB9,PT0)
SBIT(0xBA,PX1)
SBIT(0xBB,PT1)
SBIT(0xBC,PS)

/*  PSW   */
SBIT(0xD0,P)
SBIT(0xD1,F1)
SBIT(0xD2,OV)
SBIT(0xD3,RS0)
SBIT(0xD4,RS1)
SBIT(0xD5,F0)
SBIT(0xD6,AC)
SBIT(0xD7,CY)

/* BIT definitions for bits that are not directly accessible */
/* PCON bits */
#define IDL             0x01
#define PD              0x02
#define GF0             0x04
#define GF1             0x08
#define SMOD            0x80

/* TMOD bits */
#define T0_M0           0x01
#define T0_M1           0x02
#define T0_CT           0x04
#define T0_GATE         0x08
#define T1_M0           0x10
#define T1_M1           0x20
#define T1_CT           0x40
#define T1_GATE         0x80

#define T0_MASK         0x0F
#define T1_MASK         0xF0

/* Interrupt numbers: address = (number * 8) + 3 */
#define EXT0_VECTOR     0       /* 0x03 external interrupt 0 */
#define TIMER0_VECTOR   1       /* 0x0b timer 0 */
#define EXT1_VECTOR     2       /* 0x13 external interrupt 1 */
#define TIMER1_VECTOR   3       /* 0x1b timer 1 */
#define SERIAL_VECTOR   4       /* 0x23 serial port 0 */

#endif /* AT894051_H_ */
