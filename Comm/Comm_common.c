/*
 * Comm_common.c
 *
 *  Created on: Feb 18, 2013
 *      Author: dmolnar
 */

#include "Comm_common.h"


// Global variables facilitating serial communication
static unsigned char rcv_buffer[RBUFLEN], send_buffer[XBUFLEN];
static unsigned char rcv_counter, send_counter, rcv_position, send_position;
static bool UART_busy;
static unsigned char host_address;

// Global variables facilitating messaging communication
static unsigned char train_length;
static unsigned char comm_error;
static unsigned char comm_state;
static unsigned int baud_generator_interrupt_ticks;
static unsigned int message_timeout_counter;
static unsigned char CRC_burst_error_count;
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

// Measure time ticks for messaging timeout
ISR(TIMER1,0)
{
  baud_generator_interrupt_ticks+=1;
}

// Flip the bits in a byte
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


// Reset serial communication
static void reset_serial(void)
{
  // Disable serial communication
  ES = 0;

  // Setup the serial port operation mode
  // Reset receive and transmit interrupt flags and disable receiver enable
  RI  = 0;TI  = 0;
  REN = 0;
  // 8-bit UART mode for serial TIMER1 Mode2 SMOD=1
  SM1 = 1;SM0 = 0;

  // Set serial RxD and TxD lines to high
  P3_1=1; P3_0 =1;

  // Multiprocessor communication disabled
  SM2 = 0;

  // Clear serial communication buffers
  rcv_counter = send_counter = rcv_position = send_position = 0;
  UART_busy = 0;

  // Enable Serial interrupt and start listening on the bus
  ES = 1;
  REN = 1;
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

/*
 * Currently not in use - comment out to get rid of compiler mockering about it
// Is UART character transmission complete?
static char is_UART_send_complete (void)
{
   return XBUFLEN - send_counter;
}
*/

/*
 * Public functions
 */

// CRC-CCITT (0xFFFF) calculator
unsigned int calculate_CRC16(unsigned char *buf, unsigned char end_position)
{
  unsigned char i,c;
  unsigned int crc = 0xffff;
  unsigned char num;

  // Step through bytes in memory
  for (num=0; num < end_position; num++)
  {
    // Flip the bits to comply with the true serial bit order
    c = flip_bits(buf[num]);

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
  return crc; // Return updated CRC
}

// Set the communication speed of the device
void set_comm_speed(unsigned char speed)
{
  TR1  = 0; //Stop Timer 1
  TL1  = 0xff;    // Start from 255

  // Set the reload value for the baud generator
  TH1 = comm_speeds[speed].reload_value;

  // Set the SMOD bit in PCON according to speed mode
  if(comm_speeds[speed].is_smod_set) PCON |= SMOD; else PCON &= 0x7F;

  // Set the timer  mode for the baud generator Timer1
  TMOD = (TMOD&0x0f)|0x20;    // Set Timer 1 Autoreload mode

  TR1  = 1;             // Start Timer 1
  ET1  = 1;             // Enable timer1 interrupt to count ticks for messaging timeout
  reset_serial();       // Reset the communication
}

// Handle timeout events
unsigned char count_and_perform_timeout(unsigned int timeout_count_limit)
{
  // Increase timeout counter
  message_timeout_counter++;
  // If timeout limit is reached start waiting for train
  if (message_timeout_counter > timeout_count_limit)
    {
      comm_state = WAITING_FOR_TRAIN;
      comm_error = MESSAGING_TIMEOUT;
      message_buffer.index = 0;
      return 1;
    }
  return 0;
}

void reset_comm(void)
{
  // Clear message buffer
  message_buffer.index = 0;

  // Clear receiving queue
  train_length = 0;
  comm_error = NO_ERROR;
  CRC_burst_error_count = 0;

  // Set the initial state
  comm_state = WAITING_FOR_TRAIN;
  message_timeout_counter = 0;

  reset_serial();
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

// Send a message on the seria line
void send_message(unsigned char opcode, unsigned char seq)
{
  unsigned char i;
  unsigned int crc;
  message_buffer.content[LENGTH] = message_buffer.index+3;
  message_buffer.content[OPCODE] = opcode;
  message_buffer.content[SEQ] = seq;

  // Calculate the CRC
  crc = calculate_CRC16(message_buffer.content, message_buffer.index+CRC1);

  // Send the train sequence
  i=TRAIN_LENGTH_SND;
  while(i != 0)
    {
      UART_putc(TRAIN_CHR);
      i--;
    }

  // Send message body
  i = 0;
  while (i <= message_buffer.index+PARAMETER_END)
    {
      UART_putc(message_buffer.content[i]);
      i++;
    }

  UART_putc((unsigned char) ((crc & 0xff00) >> 8));
  UART_putc((unsigned char) (crc & 0x00ff));

  // Send 2 train chrs to make sure bus communication goes OK while in send mode (Practically a delay)
  UART_putc(TRAIN_CHR);
  UART_putc(TRAIN_CHR);
}

// Periodically listen for/get a message on the serial line
struct message_struct* get_message(unsigned int timeout_counter_limit)
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
      if (comm_state != WAITING_FOR_TRAIN) count_and_perform_timeout(timeout_counter_limit);
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

      // At the end of the message
      if(message_buffer.index >= message_buffer.content[0])
        {
         message_buffer.index -= 3;
        // Check the CRC of the message
        if (calculate_CRC16(message_buffer.content, message_buffer.index+CRC1) == (unsigned int)((message_buffer.content[message_buffer.index+CRC1] << 8) | (message_buffer.content[message_buffer.index+CRC2])))
        {
          // CRC is OK.
          CRC_burst_error_count = 0;
        } else {
          // CRC is wrong: set error condition
          CRC_burst_error_count++;
          comm_error = COMM_CRC_ERROR;
        }
        comm_state = WAITING_FOR_TRAIN;
        message_received = TRUE;
       }
      break;
  }

  if(message_received) return &message_buffer; else return NULL;
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

// Reset the messaging timeout counter
void reset_timeout_counter()
{
  message_timeout_counter = baud_generator_interrupt_ticks;
}

// Return if there was a messaging or response timeout
unsigned char timeout_occured(unsigned char timeout_type)
{
 unsigned char timeout, timeout_multiplier;
 unsigned int ticks_difference;

 // Set the reference timeout ticks value based on what kind of timeout status is queried
 timeout_multiplier = 1;
 if (timeout_type == RSP_TIMEOUT ) timeout_multiplier = 4;

 // If there is no owerflow in the interrupt ticks
 if (baud_generator_interrupt_ticks > message_timeout_counter)
   {
     ticks_difference = baud_generator_interrupt_ticks - message_timeout_counter;
 // There was an overflow - no multiple overflow is expected - timer must firs be reset and then queried regularily
   } else {
     ticks_difference = ((unsigned int)((unsigned int) 0xffff - message_timeout_counter)) + baud_generator_interrupt_ticks;
   }

 // If SMOD is set in PCON then HI else LO timeout value is used
 if (PCON & SMOD)
   timeout = (ticks_difference >= ((unsigned int)MSG_TIMEOUT_HI_VALUE) * timeout_multiplier);
 // comm_speed == TIMEOUT_LO
 else
   timeout = (ticks_difference >= ((unsigned int)MSG_TIMEOUT_LO_VALUE) * timeout_multiplier);

 return timeout;
}
