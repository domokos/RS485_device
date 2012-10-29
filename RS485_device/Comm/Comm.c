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
volatile unsigned char UART_received_char;
volatile bool UART_char_needs_processing;
volatile bool UART_send_complete;
volatile bool UART_char_lost;
unsigned char comm_error;
unsigned char comm_state;
unsigned char prev_char;
unsigned char host_address;
struct buffer_struct comm_buffer;


void init_comm(unsigned char _host_address)
{
  // Setup the serial port operation mode
  // Reset receive and transmit interrupt flags and disable receiver enable
  RI  = 0;TI  = 0;REN = 0;
  // 8-bit UART mode for serial TIMER1 Mode2 SMOD=1
  SM1 = 1;SM0 = 0;
  PCON |= SMOD;

  // Multiprocessor communication disabled
  SM2 = 0;

  // Setup the serial port timer Timer1
  TL1  = 0xff;    // Start from 255
  TH1  = 0xff;    // Reload 255: 57600 baud @ 11.0592 MHz Crystal with SMOD=1
  TMOD = (TMOD&0x0f)|0x20;    // Set Timer 1 Autoreload mode
  TR1  = 1;       // Start Timer 1

  // Clear receive_buffer
  comm_buffer.index = 0;

  // Clear processing queue
  UART_char_needs_processing = FALSE;
  UART_send_complete = FALSE;
  UART_char_lost = FALSE;
  comm_error = NO_ERROR;

  // Set the host address
  host_address = _host_address;

  // Set the initial state
  comm_state = AWAITING_START_FRAME;

  // Enable Serial interrupt and start listening on the bus
  ES = 1;
  REN = 1;
}

// The serial ISR for communication
void Serial_ISR(void)  __interrupt 4 __using 0
{
  if(RI == 1)
  {
    RI = 0;
    UART_received_char = SBUF;
    if (UART_char_needs_processing)
    {
    	UART_char_lost = TRUE;
    }
    else
    {
    	UART_char_needs_processing = TRUE;
    	UART_char_lost = FALSE;
    }
  }else if(TI == 1){
    TI = 0;
    UART_send_complete = TRUE;
  }
  return;
}


bool operate_comm(void)
{
	unsigned char ch_received = 0;
	bool process_char = FALSE;

	if(UART_char_needs_processing)
	{
		ES = 0; // Disable serial interrupt to make sure we are copying the correct character
		UART_char_needs_processing = FALSE;
		ch_received = UART_received_char;
		ES = 1; // Enable serial interrupt
		process_char = TRUE;
	}

	if(process_char)
	{
		switch (comm_state)
		{
		case AWAITING_START_FRAME:
		  if (ch_received == START_FRAME)
		    {
		      // Switch the state to wait for the address fileld of the frame
		      comm_state = AWAITING_ADDRESS;
		      comm_error = NO_ERROR;
		    } else {
		        // Communication error: frame out of sync set the error_condition
			comm_error = NO_START_FRAME_RECEIVED;
		    }
		  break;
		case AWAITING_ADDRESS:
		  if (ch_received == host_address)
		    {

		    } else {
		        // Not addressed to this host => Wait for next start frame

		    }
		  break;
		case RECEIVING_MESSAGE:
			//xsa;
			break;
		case MESSAGE_RECEIVED:
			//cel;
			break;
		}
/*
	  if(state == AWAITING_COMMAND)
	  {
	    if(ch_received == MESSAGE_HEAD)
	      {
			comm_buffer.index=0;
			comm_buffer.content[0]= MESSAGE_TERMINATOR;
			state = RECEIVING_COMMAND;
	      }
	  }else{
		comm_buffer.content[comm_buffer.index]=ch_received;
		comm_buffer.index++;
	  if(ch_received == MESSAGE_TERMINATOR) {state = PROCESSING_COMMAND;}
	  }
*/
		// Store prevoius char to ID escape sequences
		prev_char = process_char;
	}

	return FALSE;
}
