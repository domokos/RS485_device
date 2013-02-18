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
#include "Comm_common.h"
#include "Comm.h"

static unsigned char train_length;
static unsigned char comm_error;
static unsigned char comm_state;
static unsigned char CRC_burst_error_count;
static unsigned int message_timeout_counter;
static struct message_struct message_buffer;



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
  crc = calculate_CRC16(message_buffer.content, CRC1);

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
        if (calculate_CRC16(message_buffer.content, CRC1) == (unsigned int)((message_buffer.content[CRC1] << 8) | (message_buffer.content[CRC2])))
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
