/*
 * Comm.c
 *
 *  Created on: Oct 20, 2012
 *      Author: Molnár Domokos
 *
 *      This is a communication module for the microprocessor over
 *      RS485
 */

#include <at89x051.h>
#include "Base.h"
#include "Comm.h"

// Global variables facilitating communication
volatile unsigned char serial_received_char;
volatile __bit serial_char_needs_processing;
volatile __bit serial_send_complete;
unsigned char comm_state;
struct buffer_struct comm_buffer;


void init_comm()
{
  // Setup the serial port operation mode
  // Reset receive and transmit interrupt flags and disable receiver enable
  RI  = 0;TI  = 0;REN = 1;
  // 8-bit UART mode for serial TIMER1 Mode2 SMOD=1
  SM1 = 1;SM0 = 0;
  PCON |= SMOD;

  // Multiprocessor communication disabled
  SM2 = 0;

  // Setup the serial port timer Timer1
  TL1  = 0xff;    // Start from 255
  TH1  = 0xff;    // Reload 255 57600 baud @ 11.0592 MHz Crystal with SMOD=1
  TMOD = (TMOD&0x0f)|0x20;    // Set Timer 1 Autoreload mode
  TR1  = 1;       // Start Timer 1

  // Clear receive_buffer
  comm_buffer.index = 0;

  // Clear processing queue
  serial_char_needs_processing=FALSE;

  // Set the initial state
  comm_state = AWAITING_TRAIN_SEQ;

  // Enable Serial interrupt - start listening on the bus
  ES = 1;
}

// The serial ISR for communication
void Serial_ISR(void)  __interrupt 4 __using 0
{
  if(RI == 1)
  {
    RI = 0;
    serial_received_char = SBUF;
    serial_char_needs_processing = 1;
  }else if(TI == 1){
    TI = 0;
    serial_send_complete = 1;
  }
  return;
}

