/*
 * Comm_common.c
 *
 *  Created on: Feb 18, 2013
 *      Author: dmolnar
 */

#include "Comm_common.h"



// Global variables facilitating serial communication
static unsigned char rcv_buffer[RBUFLEN], send_buffer[XBUFLEN];
static volatile unsigned char rcv_counter, send_counter, rcv_position, send_position;
static volatile __bit UART_busy;
static unsigned char host_address;
static unsigned char comm_speed;

// Global variables facilitating messaging communication

static unsigned char comm_state;
static unsigned char CRC_burst_error_count;
static unsigned char train_length;
static unsigned char comm_error;

struct message_struct message_buffer;

/*
 * reload and timeout values in milliseconds
*/

#ifdef CRYSTAL_SPEED_LO
__code const struct comm_speed_struct comm_speeds[] = {
    {0xe8,0,120,580}, //COMM_SPEED_1200_L,SMOD not set in PCON
    {0xf4,0,60,340}, //COMM_SPEED_2400_L,SMOD not set in PCON
    {0xfa,0,30,220}, //COMM_SPEED_4800_L,SMOD not set in PCON
    {0xfd,0,15,160}, //COMM_SPEED_9600_L,SMOD not set in PCON
    {0xfe,0,14,140}, //COMM_SPEED_14400_L,SMOD not set in PCON
    {0x00,0,0,0}, //COMM_SPEED_19200_L,SMOD not set in PCON
    {0xff,0,5,120}, //COMM_SPEED_28800_L,SMOD not set in PCON
    {0x00,0,0,0}, //COMM_SPEED_57600_L,SMOD not set in PCON

    {0x40,0,0,0}, //COMM_SPEED_300_H,SMOD set in PCON
    {0xd0,0,0,0}, //COMM_SPEED_1200_H,SMOD set in PCON
    {0xe8,1,60,340}, //COMM_SPEED_2400_H,SMOD set in PCON
    {0xf4,1,30,220}, //COMM_SPEED_4800_H,SMOD set in PCON
    {0xfa,1,15,160}, //COMM_SPEED_9600_H,SMOD set in PCON
    {0xfc,1,14,140}, //COMM_SPEED_14400_H,SMOD set in PCON
    {0xfd,1,10,130}, //COMM_SPEED_19200_H,SMOD set in PCON
    {0xfe,1,5,120}, //COMM_SPEED_28800_H,SMOD set in PCON
    {0xff,1,3,110}, //COMM_SPEED_57600_H,SMOD set in PCON
    {0x00,0,0,0} //COMM_SPEED_115200_H,SMOD set in PCON
};

#elif defined CRYSTAL_SPEED_HI

__code const struct comm_speed_struct comm_speeds[] = {
    {0xd0,0,120,580}, //COMM_SPEED_1200_L,SMOD not set in PCON
    {0xe8,0,60,340}, //COMM_SPEED_2400_L,SMOD not set in PCON
    {0xf4,0,30,220}, //COMM_SPEED_4800_L,SMOD not set in PCON
    {0xfa,0,15,160}, //COMM_SPEED_9600_L,SMOD not set in PCON
    {0xfc,0,14,140}, //COMM_SPEED_14400_L,SMOD not set in PCON
    {0xfd,0,10,130}, //COMM_SPEED_19200_L,SMOD not set in PCON
    {0xfe,0,5,120}, //COMM_SPEED_28800_L,SMOD not set in PCON
    {0xff,0,3,110}, //COMM_SPEED_57600_L,SMOD not set in PCON

    {0x00,0,0,0}, //COMM_SPEED_300_H,SMOD set in PCON
    {0xa0,1,120,580}, //COMM_SPEED_1200_H,SMOD set in PCON
    {0xd0,1,60,340}, //COMM_SPEED_2400_H,SMOD set in PCON
    {0xe8,1,30,220}, //COMM_SPEED_4800_H,SMOD set in PCON
    {0xf4,1,15,160}, //COMM_SPEED_9600_H,SMOD set in PCON
    {0xf8,1,14,140}, //COMM_SPEED_14400_H,SMOD set in PCON
    {0xfa,1,10,130}, //COMM_SPEED_19200_H,SMOD set in PCON
    {0xfc,1,5,120}, //COMM_SPEED_28800_H,SMOD set in PCON
    {0xfe,1,3,110}, //COMM_SPEED_57600_H,SMOD set in PCON
    {0xff,1,2,105} //COMM_SPEED_115200_H,SMOD set in PCON
};

#else
#error "No or incorrect crystal speed defined."
#endif


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
   if (UART_busy = (send_counter>0)) {   // Assignment, _not_ comparison!
     send_counter--;
     SBUF = send_buffer [send_position++];
     if (send_position >= XBUFLEN) send_position = 0;
   }
  }
}

static unsigned char reverse_bits(unsigned char byte)
{
  unsigned char r = byte; // r will be reversed bits of v; first get LSB of v
  unsigned char s = 7; // extra shift needed at end

  for (byte >>= 1; byte; byte >>= 1)
  {
    r <<= 1;
    r |= byte & 1;
    s--;
  }

  // shift when byte's highest bits are zero
  r <<= s;
  return r;
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
Not used as of now
// Is UART buffer transmission complete?
static char is_UART_send_complete(void)
{
   return !UART_busy;
}
*/

// CRC-CCITT (0xFFFF) calculator
static unsigned int calculate_CRC16(unsigned char *buf, unsigned char end_position)
{
  unsigned char i,c;
  unsigned int crc = 0xffff;
  unsigned char num;

  // Step through bytes in memory
  for (num=0; num < end_position; num++)
  {
    // Flip the bits to comply with the true serial bit order
    c = reverse_bits(buf[num]);

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

// Handle timeout events
void evaluate_timeout()
{
  if ( timeout_occured(MSG_TIMEOUT, comm_speeds[comm_speed].msg_timeout))
    {
      comm_state = WAITING_FOR_TRAIN;
      comm_error = MESSAGING_TIMEOUT;
      message_buffer.index = 0;
    }
}


/*
 * Public functions
 */


// Set the communication speed of the device
void set_comm_speed(unsigned char speed)
{
  TR1  = 0; //Stop Timer 1
  TL1  = 0xff;    // Start from 255

  comm_speed = speed;

  // Set the reload value for the baud generator
  TH1 = comm_speeds[speed].reload_value;

  // Set the SMOD bit in PCON according to speed mode
  if(comm_speeds[speed].is_smod_set) PCON |= SMOD; else PCON &= 0x7F;

  // Set the timer  mode for the baud generator Timer1
  TMOD = (TMOD&0x0f)|0x20;    // Set Timer 1 Autoreload mode

  ET1  = 0;             // Disable timer1 interrupt to ensure smooth baud rate generation
  TR1  = 1;             // Start Timer 1
  reset_serial();       // Reset the communication
}

// Reset communication
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

// Return the comm error
unsigned char get_comm_error(void)
{
  return comm_error;
}

// Send a message on the serial line explicitely specifying SEQ
void send_message_seq(unsigned char opcode, unsigned char seq)
{
  message_buffer.content[SEQ] = seq;
  send_message(opcode);
}

// Send a message on the serial line using the SEQ just received
void send_message(unsigned char opcode)
{
  unsigned char i;
  unsigned int crc;
  message_buffer.content[LENGTH] = message_buffer.index+3;
  message_buffer.content[OPCODE] = opcode;

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

  UART_putc(TRAIN_CHR);

  while (UART_busy);
}

// Periodically listen for/get a message on the serial line
bool get_message(void)
{
  unsigned char ch_received = 0;
  bool process_char = FALSE;
  bool message_received = FALSE;

  if(UART_is_char_available())
    {
      ch_received = UART_getc();
      process_char = TRUE;
      // Reset message timeout counter as a character is received
      reset_timeout(MSG_TIMEOUT);
    } else {
      if (comm_state != WAITING_FOR_TRAIN) evaluate_timeout();
      return FALSE;
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
    // Hence the two similar states are handled together
    case RECEIVING_TRAIN:
    case IN_SYNC:
      if (ch_received == TRAIN_CHR)
        {
          // Received the expected character increase the
          // train length seen so far and change state if
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
      message_buffer.content[message_buffer.index] = ch_received;
      message_buffer.index++;

      // At the end of the message
      if(message_buffer.index == message_buffer.content[0])
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

  return message_received;
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

// Return the actual communication speed
unsigned char get_comm_speed(void)
{
  return comm_speed;
}

// Return the messaging timeout value for the actual communication speed
unsigned char get_messaging_timeout(void)
{
  return comm_speeds[comm_speed].msg_timeout;
}

// Return the response timeout value for the actual communication speed
unsigned char get_response_timeout(void)
{
  return comm_speeds[comm_speed].resp_timeout;
}
