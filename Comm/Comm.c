/*
 * Comm.c
 *
 *  Created on: Oct 20, 2012
 *      Author: Moln�r Domokos
 *
 *      This is a communication module for the AT89c4051 microcontroller
 *      over RS485
 */

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
unsigned int message_timeout_counter;
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
  prev_char = 0;

  // Set the host address
  host_address = _host_address;

  // Set the initial state
  comm_state = AWAITING_START_FRAME;
  message_timeout_counter = 0;

  // Enable Serial interrupt and start listening on the bus
  ES = 1; EA = 1;
  REN = 1;
}


unsigned char flip_bits(unsigned char byte)
{
  unsigned char flipped_byte = 0;
  char i;
  unsigned char j=7;

  for (i = 7; i >= -7;)
    {
      if (i>0)
          flipped_byte |= ((byte << i) & (0x01 << j));
        else
          flipped_byte |= ((byte >> (-i)) & (0x01 << j));
      i = i-2;
      j = j-1;
    }
  return flipped_byte;
}


// CRC-CCITT (0xFFFF)
unsigned int calculate_message_CRC16()
{
unsigned char i,c;
unsigned int crc = 0xffff;
unsigned char num;

for (num=0; num < CRC1; num++)           /* Step through bytes in memory */
{
    c = flip_bits(message_buffer.content[num]); /* Flip the bits to comply with the true serial bit order */
//	c = message_buffer.content[num];
    crc = crc ^ ((unsigned int)c << 8); /* Fetch byte from memory, XOR into  CRC top byte*/
    for (i = 0; i < 8; i++)      /* Prepare to rotate 8 bits */
    {
        if (crc & 0x8000)       /* b15 is set... */
        	crc = (crc << 1) ^ CRC16_POLYNOMIAL;    /* rotate and XOR with polynomial */
        else                     /* b15 is clear... */
            crc <<= 1;           /* just rotate */
    }                            /* Loop for 8 bits */
 }                               /* Loop until num=0 */
 return(crc);                    /* Return updated CRC */
}


// The serial ISR for communication
ISR(SERIAL,0)
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

/* Function to send response to the master on the bus
 * the function expects message content to be prepared by the caller
 * including message SEQ number
*/
void send_response(unsigned char opcode)
{
  unsigned char i;
  unsigned int crc;
  message_buffer.content[OPCODE] = opcode;

  crc = calculate_message_CRC16();
  message_buffer.content[CRC1] = (unsigned char) ((crc & 0xff00) >> 8);
  message_buffer.content[CRC2] = (unsigned char) (crc & 0x00ff);
  // Now send the message
  // Frame head first
   UART_putchar(START_FRAME);

   // Send message body
  for (i=SLAVE_ADDRESS; i<=CRC2;i++)
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
      switch (comm_state) {
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
            escape_char_recieved = FALSE;
            // Clear message buffer
            message_buffer.index = 0;
          }else if (ch_received == MESSAGE_ESCAPE && !escape_char_recieved) {
            escape_char_recieved = TRUE;
            comm_error = NO_ERROR;
          }else if (ch_received != END_FRAME || escape_char_recieved) {
            // Receive the message character and clear the escape flag
            message_buffer.content[message_buffer.index]=ch_received;
            message_buffer.index++;
            escape_char_recieved = FALSE;
            comm_error = NO_ERROR;
          }else {
            // End frame received change state to post processing
            comm_state = AWAITING_START_FRAME;
            // Decrease index to point to the last byte of the message payload
            // Excluding CRC bytes
            message_buffer.index -= 3;
            if (calculate_message_CRC16() == (unsigned int)((message_buffer.content[CRC1] << 8) | (message_buffer.content[CRC2])))
            {
            	CRC_burst_error_count = 0;
            	message_recieved = TRUE;
            	comm_error = NO_ERROR;
            } else {
            	// Send error response if this host is the addressee
            	if(host_address == message_buffer.content[SLAVE_ADDRESS])
            	  {
            	    // Send no parameter in the message
            	    message_buffer.index = PARAMETER_START-1;
            	    send_response(CRC_ERROR);
            	  }
            	// Clear message buffer
            	message_buffer.index = 0;
            	CRC_burst_error_count++;
            	comm_error = COMM_CRC_ERROR;
            }
          }
        break;
      }
      // Store previous char to ID escape sequences
      prev_char = process_char;
    } else {
    	if( comm_state == RECEIVING_MESSAGE )
    	{
    	    if (message_timeout_counter++ > MESSAGE_TIMEOUT_COUNT_LIMIT )
    	      {
    	        message_timeout_counter = 0;
    	        comm_state =  AWAITING_START_FRAME;
    	        message_buffer.index = 0;
    	        comm_error = MESSAGING_TIMEOUT;
    	      }
    	}

    }
  if(message_recieved) return &message_buffer; else return 0;
}
