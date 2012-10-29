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


void clear_message_buffer()
{
  // Clear receive_buffer
  message_buffer.index = 0;
  message_buffer.address = 0;
  message_buffer.seq = 0;
}

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

  clear_message_buffer();

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
unsigned char byteCtr;
unsigned char bit_nr;

// Loop through the message bytes
for (byteCtr = 0; byteCtr < message_buffer.index; byteCtr++)
  {
    // Pick the next byte for processing
    crc ^= (message_buffer.content[byteCtr]);
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
struct message_struct* get_message()
{
  return &message_buffer;
}

// Send acknowledge message indicating success according to parameter
void ack_message(bool success)
{
  if (success)
    {

    } else {

    }
}

// Return TRUE if valid message to be processed is recieved
// return FALSE otherwise
bool operate_comm(void)
{
  unsigned char ch_received = 0;
  bool process_char = FALSE;
  bool valid_message_recieved = FALSE;

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
            comm_state = RECEIVING_MESSAGE;
            comm_error = NO_ERROR;
          } else {
              // Communication error: frame out of sync set the error_condition
              comm_error = NO_START_FRAME_RECEIVED;
          }
        break;
      case RECEIVING_MESSAGE:
        if (ch_received == MESSAGE_ESCAPE && escape_char_recieved == FALSE)
          {
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
        if (calculate_message_CRC() == message_buffer.content[message_buffer.index])
          {
            ack_message(TRUE);
            CRC_burst_error_count = 0;
            // Todo: Work out address ans SEQ handling
            comm_state = AWAITING_START_FRAME;
            valid_message_recieved = TRUE;
          } else {
            ack_message(FALSE);
            clear_message_buffer();
            CRC_burst_error_count++;
            comm_state = AWAITING_START_FRAME;
          }
        break;
      }
      // Store prevoius char to ID escape sequences
      prev_char = process_char;
    }
  return valid_message_recieved;
}
