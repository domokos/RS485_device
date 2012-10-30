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
bool escape_char_recieved;
unsigned char current_message_address;
unsigned char comm_error;
unsigned char comm_state;
unsigned char prev_char;
unsigned char host_address;
unsigned char CRC_burst_error_count;
struct message_struct message_buffer;


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

  // Clear message buffer
  message_buffer.index = 0;

  // Clear processing queue
  UART_char_needs_processing = FALSE;
  UART_send_complete = FALSE;
  UART_char_lost = FALSE;
  comm_error = NO_ERROR;
  escape_char_recieved = FALSE;
  CRC_burst_error_count = 0;

  // Set the host address
  host_address = _host_address;

  // Set the initial state
  comm_state = AWAITING_START_FRAME;

  // Enable Serial interrupt and start listening on the bus
  ES = 1;
  REN = 1;
}

// Calculate the 8-bit CRC for the message payload
unsigned char calculate_message_CRC()
{
unsigned char crc = 0;
unsigned char byte_nr;
unsigned char bit_nr;

// Loop through the message bytes
for (byte_nr = 0; byte_nr < message_buffer.index; byte_nr++)
  {
    // Pick the next byte for processing
    crc ^= (message_buffer.content[byte_nr]);
    // Loop through the bits of the byte to be processed
    for (bit_nr = 8; bit_nr > 0; bit_nr--)
      {
        // Compare the bit with bit 7
        // If the same: shift CRC register, bit0='0'
        // else: shift CRC register and then invert the polynomial's set bits
        if (crc & 0x80) crc = (crc << 1) ^ CRC_POLYNOMIAL;
                   else crc = (crc << 1);
      }
  }
return crc;
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

// Provide access to the message structure
struct message_struct* get_message_buffer(void)
{
  return &message_buffer;
}


// Send a character to the UART
void UART_putchar(unsigned char value)
{
  UART_send_complete = FALSE;
  SBUF = value;
  do{
/*
 * The serial interrupt handler will set the UARTsend_complete flag
 * Optimizer will let us do this as the variable is declared as volatile
*/
    }while(!UART_send_complete);
}

// Function to send response to the master on the bus
// the function expects message content to be prepared by the caller
// including message SEQ number
void send_response(unsigned char opcode)
{
  unsigned char i;
  message_buffer.content[OPCODE] = opcode;
  message_buffer.content[CRC] = calculate_message_CRC();
  // Now send the message
  // Frame head first
   UART_putchar(START_FRAME);

   // Send message body
  for (i=PARAMETER_START; i<=PARAMETER_END;i++)
    {
      // Escape special characters
      if (message_buffer.content[i] == MESSAGE_ESCAPE ||
          message_buffer.content[i] == START_FRAME ||
          message_buffer.content[i] == END_FRAME)
            UART_putchar(MESSAGE_ESCAPE);

      UART_putchar(message_buffer.content[i]);
    }

  // Send frame end
  UART_putchar(END_FRAME);
}

// Returns void* to the caller if no message is received
// returns a pointer to the message if a message is received
struct message_struct* get_message(void)
{
  unsigned char ch_received = 0;
  bool process_char = FALSE;
  bool message_recieved = FALSE;

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
        if (ch_received == START_FRAME && prev_char != MESSAGE_ESCAPE)
          {
            // Switch the state to wait for the address fileld of the frame
            comm_state = RECEIVING_MESSAGE;
            comm_error = NO_ERROR;
          } else {
            // Communication error: frame out of sync set the error_condition
            comm_error = NO_START_FRAME_RECEIVED;

          }
        break;
      case RECEIVING_MESSAGE:
        if(message_buffer.index > MAX_MESSAGE_LENGTH-1)
          {
            // Set error, start waiting for next message head and ignore the rest of the message
            comm_error = MESSAGE_TOO_LONG;
            comm_state = AWAITING_START_FRAME;
            // Clear message buffer
            message_buffer.index = 0;
          } else if (ch_received == MESSAGE_ESCAPE && escape_char_recieved == FALSE) {
            escape_char_recieved = TRUE;
          } else if (ch_received != END_FRAME){
              // Recieve the message character and clear the escape flag
              message_buffer.content[message_buffer.index]=ch_received;
              message_buffer.index++;
              escape_char_recieved = FALSE;
          } else {
              // End frame recieved change state to post processing
              escape_char_recieved = FALSE;
              comm_state = POSTPROCESSING_MESSAGE;
              // Decrease index to point to the last byte of the message payload
              message_buffer.index--;
          }
        break;
      case POSTPROCESSING_MESSAGE:
        if (calculate_message_CRC() == message_buffer.content[CRC])
          {
            CRC_burst_error_count = 0;
            comm_state = AWAITING_START_FRAME;
            message_recieved = TRUE;
          } else {
            // Send error response if this host is the addressee
            if(host_address == message_buffer.content[SLAVE_ADDRESS]) send_response(CRC_ERROR);
            // Clear message buffer
            message_buffer.index = 0;
            CRC_burst_error_count++;
            comm_state = AWAITING_START_FRAME;
          }
        break;
      }
      // Store prevoius char to ID escape sequences
      prev_char = process_char;
    }
  if(message_recieved) return &message_buffer; else return 0;
}
