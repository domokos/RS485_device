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
static bool escape_char_received;
static unsigned char train_length;
static unsigned char comm_error;
static unsigned char comm_state;
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

// Low level flood test of the bus
void bus_flood_test(unsigned char character, int repeat)
{
 set_comm_direction(DEVICE_SENDS);
 while(repeat--)
   {
     UART_send_complete = FALSE;
      SBUF = character;
      do{
        }while(!UART_send_complete);
   }
 set_comm_direction(DEVICE_LISTENS);
}

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
  do{
/*
 * The serial interrupt handler will set the UARTsend_complete flag
 * Optimizer will let us do this as the variable is declared as volatile
*/
    }while(!UART_send_complete);
  UART_send_complete = FALSE;
  SBUF = value;
}

// Set the direction of communication
static void set_comm_direction(unsigned char direction)
{
  COMM_DIRECTION_PIN = direction;
}

static void count_and_perform_timeout(void)
{
  // Increase timeout counter
  message_timeout_counter++;
  // If timeout limit is reached start waiting for train
  if (message_timeout_counter > MESSAGE_TIMEOUT_COUNT_LIMIT)
    {
      comm_state = WAITING_FOR_TRAIN;
      comm_error = MESSAGING_TIMEOUT;
      message_buffer.index = 0;
    }
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
  train_length = 0;
  comm_error = NO_ERROR;
  escape_char_received = FALSE;
  CRC_burst_error_count = 0;

  // Set the initial state
  comm_state = WAITING_FOR_TRAIN;
  message_timeout_counter = 0;

  // Listen on the bus for commands
  set_comm_direction(DEVICE_LISTENS);
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

  // Reset SW flags of the UART
  UART_char_needs_processing = FALSE;
  UART_send_complete = TRUE;
  UART_char_lost = FALSE;

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

  // Calculate the CRC
  crc = calculate_message_CRC16();

  // Now send the message
  set_comm_direction(DEVICE_SENDS);

  // Send the train sequence
  for(i=0; i<TRAIN_LENGTH_SND; i++)
    {
      UART_putchar(TRAIN_CHR);
    }

   // Send message body
  for (i=SLAVE_ADDRESS; i<CRC1; i++)
    {
      // Escape special characters
      if (message_buffer.content[i] == ESCAPE_CHR ||
          message_buffer.content[i] == TRAIN_CHR)
            UART_putchar(ESCAPE_CHR);

      UART_putchar(message_buffer.content[i]);
    }

  // Send a train chr to indicate end of message
  UART_putchar(TRAIN_CHR);

  UART_putchar((unsigned char) ((crc & 0xff00) >> 8));
  UART_putchar((unsigned char) (crc & 0x00ff));

  // Send a train chr to make sure bus communication goes OK while in send mode (Practically a delay)
  UART_putchar(TRAIN_CHR);

  // Listen on the bus
  set_comm_direction(DEVICE_LISTENS);
}

// Returns void* to the caller if no message is received
// returns a pointer to the message if a message is received
// Logically clean but suboptimal implementation
struct message_struct* get_message(void)
{
  unsigned char ch_received = 0;
  bool process_char = FALSE;
  bool message_received = FALSE;

  if(UART_char_needs_processing)
    {
      ES = 0; // Disable serial interrupt to make sure we are copying the correct character
      UART_char_needs_processing = FALSE;
      ch_received = UART_received_char;
      ES = 1; // Enable serial interrupt
      process_char = TRUE;
      // Reset message timeout counter as a character is received
      message_timeout_counter = 0;
    }

  switch (comm_state) {
    case WAITING_FOR_TRAIN:
      if(process_char)
        {
        if (ch_received == TRAIN_CHR)
          {
            // Switch the state to wait for the address fileld of the frame
            // reset the next state by setting train_length to zero
            train_length = 0;
            comm_state = RECEIVING_TRAIN;
            comm_error = NO_ERROR;
          } else {
            // Communication error: frame out of sync set the error_condition and
            // do not change communication state: keep waiting for a start frame.
            comm_error = NO_TRAIN_RECEIVED;
          }
        } else {
         // Do nothing stay in current state
        }
      break;

    case IN_SYNC:
    case RECEIVING_TRAIN:
      if(process_char)
        {
        if (ch_received == TRAIN_CHR)
          {
            // Recieved the expected character increase the
            // train length seen so far and cahge state if
            // enough train is seen
            train_length++;
            if(train_length == TRAIN_LENGTH_RCV)
              {
                comm_state = IN_SYNC;
              }
          } else {
            if (comm_state ==  RECEIVING_TRAIN)
              {
                // Not a train character is received, not yet synced
                // Go back to Waiting for train state
                comm_state = WAITING_FOR_TRAIN;
                comm_error = NO_TRAIN_RECEIVED;
              } else {
                // Got a non-train character when synced -
                // start processig the message: change state
                comm_state = RECEIVING_MESSAGE;
                message_buffer.content[0]=ch_received;
                message_buffer.index = 1;
                escape_char_received = FALSE;
              }
          }
        } else count_and_perform_timeout();
      break;

    case RECEIVING_MESSAGE:
      if(process_char)
        {
          if(message_buffer.index > MAX_MESSAGE_LENGTH-1)
            {
              // Set error, start waiting for next train, ignore the rest of the message
              // and clear the message buffer
              comm_error = MESSAGE_TOO_LONG;
              comm_state = WAITING_FOR_TRAIN;
              escape_char_received = FALSE;
              message_buffer.index = 0;

            }else if (ch_received == ESCAPE_CHR && !escape_char_received && message_buffer.index > 2) {
              // Set the escape flag and wait for the next character
              escape_char_received = TRUE;

            } else if (ch_received == TRAIN_CHR && !escape_char_received && message_buffer.index > 2) {
              // Train character received start receiving the 2 CRC bytes
                comm_state = RECEIVING_CRC1;

            }else {
              // Receive the escaped or not escaped message character
              // and clear the escape flag
              message_buffer.content[message_buffer.index]=ch_received;
              message_buffer.index++;
              escape_char_received = FALSE;

            }
        } else count_and_perform_timeout();
      break;

    case RECEIVING_CRC1:
    case RECEIVING_CRC2:
      if(process_char)
        {
          // If character is received and it was the first CRC character then
          // get the first CRC and proceed on waiting for the next character
          if (comm_state == RECEIVING_CRC1)
            {
              // Fix message buffer indes so that it points to the
              // last message character recieved
              message_buffer.index--;
              message_buffer.content[CRC1] = ch_received;
              comm_state = RECEIVING_CRC2;
              break;
            }

          message_buffer.content[CRC2] = ch_received;

          // Check the CRC of the message
          if (calculate_message_CRC16() == (unsigned int)((message_buffer.content[CRC1] << 8) | (message_buffer.content[CRC2])))
          {
              // CRC is OK.
              CRC_burst_error_count = 0;
              message_received = TRUE;
          } else {
              // CRC is wrong: send error response if this host is the addressee
              if(host_address == message_buffer.content[SLAVE_ADDRESS])
                {
                  // Send no parameter in the message
                  message_buffer.index = PARAMETER_START-1;
                  send_response(CRC_ERROR,message_buffer.content[SEQ]);
                }
              CRC_burst_error_count++;
              comm_error = COMM_CRC_ERROR;
          }
          comm_state = WAITING_FOR_TRAIN;
        } else count_and_perform_timeout();
      break;
  }

  if(message_received) return &message_buffer; else return NULL;
}
