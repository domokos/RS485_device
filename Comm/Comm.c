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


static unsigned char rcv_buffer[RBUFLEN], send_buffer[XBUFLEN];
static unsigned char rcv_counter, send_counter, rcv_position, send_position;
static bool UART_busy;

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


// CRC-CCITT (0xFFFF) calculator
static unsigned int calculate_message_CRC16()
{
unsigned char i,c;
unsigned int crc = 0xffff;
unsigned char num;

// Step through bytes in memory
for (num=0; num < CRC1; num++)
{
    // Flip the bits to comply with the true serial bit order
    c = flip_bits(message_buffer.content[num]);

    // Fetch byte from memory, XOR into  CRC top byte
    crc = crc ^ ((unsigned int)c << 8);

    // Prepare to rotate 8 bits
    for (i = 0; i < 8; i++)
    {
        // b15 is set...
        if (crc & 0x8000)
            // rotate and XOR with polynomial
            crc = (crc << 1) ^ CRC16_POLYNOMIAL;
        else                     // b15 is clear...
            // just rotate
            crc <<= 1;
    } // Loop for 8 bits
 } // Loop until num=0
 return(crc); // Return updated CRC
}

// The serial ISR for communication
ISR(SERIAL,0)
{
  if (RI) {
   RI = 0;
   // Don't overwrite chars already in buffer
   if (rcv_counter < RBUFLEN) rcv_buffer [(unsigned char)(rcv_position+rcv_counter++) % RBUFLEN] = SBUF;
  }
  if (TI) {
   TI = 0;
   if (UART_busy = send_counter) {   // Assignment, _not_ comparison!
     send_counter--;
     SBUF = send_buffer [send_position++];
     if (send_position >= XBUFLEN) send_position = 0;
   }
  }
}

// Send a character to the UART
static void UART_putc(unsigned char c)
{
  // Wait for room in buffer
  while (send_counter >= XBUFLEN);
  ES = 0;
  if (UART_busy) {
    send_buffer[(unsigned char)(send_position+send_counter++) % XBUFLEN] = c;
  } else {
    SBUF = c;
    UART_busy = 1;
  }
  ES = 1;
}

// Read a character from the UART buffer
static unsigned char UART_getc(void)
{
  unsigned char c;
  // Wait for a character
  while (!rcv_counter);
  ES = 0;
  rcv_counter--;
  c = rcv_buffer [rcv_position++];
  if (rcv_position >= RBUFLEN)
         rcv_position = 0;
  ES = 1;
  return c;
}

// Are there any caharcters in the UART buffer available for reading?
static unsigned char UART_is_char_available(void)
{
   return rcv_counter;
}

// Is UART character transmission complete?
unsigned char is_UART_send_complete (void)
{
   return XBUFLEN - send_counter;
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
 * Public interface functions
 */

// Reset the state of the communication channel
void reset_comm(void)
{
  __bit ES_tmp = ES;
  ES_tmp = ES;

  ES = 0;
  // Clear message buffer
  message_buffer.index = 0;

  // Clear receiving queue
  train_length = 0;
  comm_error = NO_ERROR;
  CRC_burst_error_count = 0;

  // Set the initial state
  comm_state = WAITING_FOR_TRAIN;
  message_timeout_counter = 0;

  // Clear serial communication buffers
  rcv_counter = send_counter = rcv_position = send_position = 0;
  UART_busy = 0;

  // Listen on the bus for commands
  set_comm_direction(DEVICE_LISTENS);

  ES = ES_tmp;
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

// Set the communication speed of the device
void set_comm_speed(unsigned char comm_speed)
{
  TR1  = 0; //Stop Timer 1
  TL1  = 0xff;    // Start from 255
  TH1 = comm_speeds[comm_speed].reload_value;
  if(comm_speeds[comm_speed].is_smod_set) PCON|=SMOD; else PCON&=0x7F;
  // Setup the serial port timer Timer1
  TMOD = (TMOD&0x0f)|0x20;    // Set Timer 1 Autoreload mode
  TR1  = 1;       // Start Timer 1
  reset_comm(); // Reset the communication
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

/* Function to send response to the master on the bus
 * the function expects message content to be prepared by the caller
 * including message SEQ number
*/
void send_response(unsigned char opcode, unsigned char seq)
{
  unsigned char i,j;
  unsigned int crc;
  message_buffer.content[LENGTH] = message_buffer.index+3;
  message_buffer.content[OPCODE] = opcode;
  message_buffer.content[SEQ] = seq;

  // Calculate the CRC
  crc = calculate_message_CRC16();

  // Now send the message
  set_comm_direction(DEVICE_SENDS);

  // Send the train sequence
  for(j=0; j<TRAIN_LENGTH_SND; j++)
    {
      UART_putc(TRAIN_CHR);
    }

  // Send message body
  i = 0;
  while (i<=PARAMETER_END)
    {
      UART_putc(message_buffer.content[i]);
      i++;
    }

  UART_putc((unsigned char) ((crc & 0xff00) >> 8));
  UART_putc((unsigned char) (crc & 0x00ff));

  // Send 2 train chrs to make sure bus communication goes OK while in send mode (Practically a delay)
  UART_putc(TRAIN_CHR);
  UART_putc(TRAIN_CHR);

  // Listen on the bus
  set_comm_direction(DEVICE_LISTENS);
}

// Returns void* to the caller if no message is received
// returns a pointer to the message if a message is received
struct message_struct* get_message(void)
{
  unsigned char ch_received = 0;
  bool process_char = FALSE;
  bool message_received = FALSE;

  if(UART_is_char_available())
    {
      ch_received = UART_getc();
      process_char = TRUE;
      // Reset message timeout counter as a character is received
      message_timeout_counter = 0;
    } else {
      if (comm_state != WAITING_FOR_TRAIN) count_and_perform_timeout();
      return NULL;
    }

  switch (comm_state) {
    case WAITING_FOR_TRAIN:
      if (ch_received == TRAIN_CHR)
        {
          // Switch the state to wait for the address fileld of the frame
          // reset the next state by setting train_length to zero
          train_length = 0;
          comm_state = RECEIVING_TRAIN;
          comm_error = NO_ERROR;
        } else {
          // Communication error: frame out of sync set the error_condition and
          // do not change communication state: keep waiting for a train character.
          comm_error = NO_TRAIN_RECEIVED;
        }
      break;

    // Optimizing for smaller code size for the microcontroller -
    // hece the two similar states are handled together
    case RECEIVING_TRAIN:
    case IN_SYNC:
      if (ch_received == TRAIN_CHR)
        {
          // Recieved the expected character increase the
          // train length seen so far and cahge state if
          // enough train is seen
          if (train_length < TRAIN_LENGTH_RCV)
            {
              train_length++;
            }else {
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
              // this is the message length check it and if OK, start receiving
              if(ch_received > MAX_MESSAGE_LENGTH)
                {
                  // Set error, start waiting for next train, ignore the rest of the message
                  // and clear the message buffer
                  comm_error = MESSAGE_TOO_LONG;
                  comm_state = WAITING_FOR_TRAIN;
                } else {
                  // Clear message buffer and start recieving
                  comm_state = RECEIVING_MESSAGE;
                  message_buffer.content[0] = ch_received;
                  message_buffer.index = 1;
                }
            }
        }
      break;

    case RECEIVING_MESSAGE:
      // Receive the next message character
      message_buffer.content[message_buffer.index]=ch_received;
      message_buffer.index++;

      // It at the end of the message check it
      if(message_buffer.index >= message_buffer.content[0])
        {
         message_buffer.index -= 3;
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
       }
      break;
  }

  if(message_received) return &message_buffer; else return NULL;
}

// Low level flood test of the bus
void bus_flood_test(unsigned char character, int repeat)
{
 set_comm_direction(DEVICE_SENDS);
 while(repeat--)
   {
     UART_putc(character);
   }
 set_comm_direction(DEVICE_LISTENS);
}

// Return the # of CRC errors seen
unsigned char get_CRC_burst_error_count(void)
{
  return CRC_burst_error_count;
}

// Return the state of the communication
unsigned char get_comm_state(void)
{
  return comm_state;
}

// Return train length seen
unsigned char get_train_length(void)
{
  return train_length;
}
