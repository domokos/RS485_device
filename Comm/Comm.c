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
static volatile unsigned char UART_received_char;
static volatile bool UART_char_needs_processing;
static volatile bool UART_send_complete;
static volatile bool UART_char_lost;
static bool escape_char_recieved;
static unsigned char comm_error;
static unsigned char comm_state;
static unsigned char prev_char;
static unsigned char host_address;
static unsigned char CRC_burst_error_count;
static unsigned int message_timeout_counter;
static struct message_struct message_buffer;
__code static const struct comm_speed_struct comm_speeds[] = {
    {0xa0,0}, //COMM_SPEED_300_L 0x40,SMOD not set in PCON
    {0xe8,0}, //COMM_SPEED_1200_L 0xe8,SMOD not set in PCON
    {0xf4,0}, //COMM_SPEED_2400_L 0xf4,SMOD not set in PCON
    {0xfa,0}, //COMM_SPEED_4800_L 0xfa,SMOD not set in PCON
    {0xfd,0}, //COMM_SPEED_9600_L 0xfd,SMOD not set in PCON
    {0xfe,0}, //COMM_SPEED_14400_L 0xfe,SMOD not set in PCON
    {0xff,0}, //COMM_SPEED_28800_L 0xff,SMOD not set in PCON

    {0x40,1}, //COMM_SPEED_300_H 0x40,SMOD set in PCON
    {0xd0,1}, //COMM_SPEED_1200_H 0xd0,SMOD set in PCON
    {0x8e,1}, //COMM_SPEED_2400_H 0x8e,SMOD set in PCON
    {0xf4,1}, //COMM_SPEED_4800_H 0xf4,SMOD set in PCON
    {0xfa,1}, //COMM_SPEED_9600_H 0xfa,SMOD set in PCON
    {0xfc,1}, //COMM_SPEED_14400_H 0xfc,SMOD set in PCON
    {0xfd,1}, //COMM_SPEED_19200_H 0xfd,SMOD set in PCON
    {0xfe,1}, //COMM_SPEED_28800_H 0xfe,SMOD set in PCON
    {0xff,1} //COMM_SPEED_57600_H 0xff,SMOD set in PCON
};

/*
 * Internal utility functions
 */

static unsigned char flip_bits(unsigned char byte)
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
static unsigned int calculate_message_CRC16()
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


// Send a character to the UART
static void UART_putchar(unsigned char value)
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


/*
 * Public interface
 */

// Reset the state of the communication channel
void reset_comm(void)
{
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

  // Set the initial state
  comm_state = AWAITING_START_FRAME;
  message_timeout_counter = 0;
}

// Initialize the communication module
void init_comm(unsigned char host_address, unsigned char comm_speed)
{
  // Setup the serial port operation mode
  // Reset receive and transmit interrupt flags and disable receiver enable
  RI  = 0;TI  = 0;REN = 0;
  // 8-bit UART mode for serial TIMER1 Mode2 SMOD=1
  SM1 = 1;SM0 = 0;

  // Set serial RxD and TxD lines to high
  P3_1=1; P3_0 =1;

  // Multiprocessor communication disabled
  SM2 = 0;

  // Set the communication speed
  set_comm_speed(comm_speed);

  // Set the host address
  set_host_address(host_address);

  // Reset the communication channel
  reset_comm();

  // Enable Serial interrupt and start listening on the bus
  ES = 1; EA = 1;
  REN = 1;
}

void set_comm_speed(unsigned char comm_speed)
{
  TR1  = 0; //Stop Timer 1
  TL1  = 0xff;    // Start from 255
  TH1 = comm_speeds[comm_speed].reload_value;
  if(comm_speeds[comm_speed].is_smod_set) PCON|=SMOD; else PCON&=0x7F;
  // Setup the serial port timer Timer1
  TMOD = (TMOD&0x0f)|0x20;    // Set Timer 1 Autoreload mode
  TR1  = 1;       // Start Timer 1
}

// Provide access to the message structure
struct message_struct* get_message_buffer(void)
{
  return &message_buffer;
}

// Return the comm error
unsigned char get_comm_error(void)
{
  return comm_error;
}

// Indicate if UART has lost a char
bool is_UART_char_lost(void)
{
  return UART_char_lost;
}

// Return the host address
unsigned char get_host_address(void)
{
  return host_address;
}

// Set the host address
void set_host_address(unsigned char _host_address)
{
  host_address = _host_address;
}

// Return the # of CRC errors seen
unsigned char get_CRC_burst_error_count(void)
{
  return CRC_burst_error_count;
}

/* Function to send response to the master on the bus
 * the function expects message content to be prepared by the caller
 * including message SEQ number
*/
void send_response(unsigned char opcode, unsigned char seq)
{
  unsigned char i;
  unsigned int crc;
  message_buffer.content[OPCODE] = opcode;
  message_buffer.content[SEQ] = seq;

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
      // A character is recieved by UART - process it
      message_timeout_counter = 0;
      switch (comm_state) {
      case AWAITING_START_FRAME:
        if (ch_received == START_FRAME && prev_char != MESSAGE_ESCAPE)
          {
            // Switch the state to wait for the address fileld of the frame
            comm_state = RECEIVING_MESSAGE;
            comm_error = NO_ERROR;
          } else {
            // Communication error: frame out of sync set the error_condition and
            // do not change communication state: keep waiting for a start frame.
            comm_error = NO_START_FRAME_RECEIVED;
          }
        break;
      case RECEIVING_MESSAGE:
        if(message_buffer.index > MAX_MESSAGE_LENGTH-1)
          {
            // Set error, start waiting for next message head, ignore the rest of the message
            // and clear the message buffer
            comm_error = MESSAGE_TOO_LONG;
            comm_state = AWAITING_START_FRAME;
            escape_char_recieved = FALSE;
            message_buffer.index = 0;

          }else if (ch_received == MESSAGE_ESCAPE && !escape_char_recieved) {
            // Set the escape flag and wait for the next character
            escape_char_recieved = TRUE;
            comm_error = NO_ERROR;

          }else if (ch_received != END_FRAME || escape_char_recieved) {
            // Receive the message character and clear the escape flag
            message_buffer.content[message_buffer.index]=ch_received;
            message_buffer.index++;
            escape_char_recieved = FALSE;
            comm_error = NO_ERROR;

          }else {
            // End frame received change state to waiting for the next message
            // and perform post processing
            comm_state = AWAITING_START_FRAME;

            // Decrease index to point to the last byte of the message payload
            // excluding CRC bytes
            message_buffer.index -= 3;
            if (calculate_message_CRC16() == (unsigned int)((message_buffer.content[CRC1] << 8) | (message_buffer.content[CRC2])))
            {
                // CRC is OK.
            	CRC_burst_error_count = 0;
            	message_recieved = TRUE;
            	comm_error = NO_ERROR;

            } else {
            	// CRC is wrong: send error response if this host is the addressee
            	if(host_address == message_buffer.content[SLAVE_ADDRESS])
            	  {
            	    // Send no parameter in the message
            	    message_buffer.index = PARAMETER_START-1;
            	    send_response(CRC_ERROR,message_buffer.content[SEQ]);
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
        // No character is recieved by UART
        // maintain a timeout counter and cleanup communication if
        // no transmission is seen on the line
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
  if(message_recieved) return &message_buffer; else return NULL;
}
