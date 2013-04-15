/*
 * Comm_common.c
 *
 *  Created on: Feb 18, 2013
 *      Author: dmolnar
 */

#include "Comm_common.h"

// Global variables facilitating serial communication
static unsigned char send_buffer[UART_XBUFLEN];
static volatile unsigned char send_counter, send_position;
static volatile bool UART_busy, message_awaits_processing, timeout_flag;
static bool msg_timeout_active;
static unsigned char host_address;
static unsigned char comm_speed;

// Global variables facilitating messaging communication

static unsigned char CRC_error_count;
static volatile unsigned char comm_error, comm_state, train_char_seen;

struct message_struct message_buffer;

/*
 * reload and timeout values in milliseconds
 */

#ifdef CRYSTAL_SPEED_LO
__code const struct comm_speed_struct comm_speeds[] =
  {
      { 0xe8, 0, 150, 700}, //COMM_SPEED_1200_L,SMOD not set in PCON
      { 0xf4, 0, 80, 420}, //COMM_SPEED_2400_L,SMOD not set in PCON
      { 0xfa, 0, 40, 260}, //COMM_SPEED_4800_L,SMOD not set in PCON
      { 0xfd, 0, 25, 200}, //COMM_SPEED_9600_L,SMOD not set in PCON
      { 0xfe, 0, 20, 180}, //COMM_SPEED_14400_L,SMOD not set in PCON
      { 0x00, 0, 0, 0}, //COMM_SPEED_19200_L,SMOD not set in PCON
      { 0xff, 0, 10, 140}, //COMM_SPEED_28800_L,SMOD not set in PCON
      { 0x00, 0, 0, 0}, //COMM_SPEED_57600_L,SMOD not set in PCON

      { 0x40, 0, 0, 0}, //COMM_SPEED_300_H,SMOD set in PCON
      { 0xd0, 0, 0, 0}, //COMM_SPEED_1200_H,SMOD set in PCON
      { 0xe8, 1, 80, 420}, //COMM_SPEED_2400_H,SMOD set in PCON
      { 0xf4, 1, 40, 260}, //COMM_SPEED_4800_H,SMOD set in PCON
      { 0xfa, 1, 25, 200}, //COMM_SPEED_9600_H,SMOD set in PCON
      { 0xfc, 1, 20, 180}, //COMM_SPEED_14400_H,SMOD set in PCON
      { 0xfd, 1, 18, 172}, //COMM_SPEED_19200_H,SMOD set in PCON
      { 0xfe, 1, 10, 140}, //COMM_SPEED_28800_H,SMOD set in PCON
      { 0xff, 1, 6, 124}, //COMM_SPEED_57600_H,SMOD set in PCON
      { 0x00, 0, 0, 0} //COMM_SPEED_115200_H,SMOD set in PCON
  };

#elif defined CRYSTAL_SPEED_HI

__code const struct comm_speed_struct comm_speeds[] =
  {
    { 0xd0, 0, 150, 700 }, //COMM_SPEED_1200_L,SMOD not set in PCON
        { 0xe8, 0, 80, 420 }, //COMM_SPEED_2400_L,SMOD not set in PCON
        { 0xf4, 0, 40, 260 }, //COMM_SPEED_4800_L,SMOD not set in PCON
        { 0xfa, 0, 25, 200 }, //COMM_SPEED_9600_L,SMOD not set in PCON
        { 0xfc, 0, 20, 180 }, //COMM_SPEED_14400_L,SMOD not set in PCON
        { 0xfd, 0, 18, 172 }, //COMM_SPEED_19200_L,SMOD not set in PCON
        { 0xfe, 0, 10, 140 }, //COMM_SPEED_28800_L,SMOD not set in PCON
        { 0xff, 0, 6, 124 }, //COMM_SPEED_57600_L,SMOD not set in PCON

        { 0x00, 0, 0, 0 }, //COMM_SPEED_300_H,SMOD set in PCON
        { 0xa0, 1, 150, 700 }, //COMM_SPEED_1200_H,SMOD set in PCON
        { 0xd0, 1, 80, 420 }, //COMM_SPEED_2400_H,SMOD set in PCON
        { 0xe8, 1, 40, 260 }, //COMM_SPEED_4800_H,SMOD set in PCON
        { 0xf4, 1, 25, 200 }, //COMM_SPEED_9600_H,SMOD set in PCON
        { 0xf8, 1, 20, 180 }, //COMM_SPEED_14400_H,SMOD set in PCON
        { 0xfa, 1, 18, 172 }, //COMM_SPEED_19200_H,SMOD set in PCON
        { 0xfc, 1, 10, 140 }, //COMM_SPEED_28800_H,SMOD set in PCON
        { 0xfe, 1, 6, 124 }, //COMM_SPEED_57600_H,SMOD set in PCON
        { 0xff, 1, 4, 116 } //COMM_SPEED_115200_H,SMOD set in PCON
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
    unsigned char chr_received;
    if (RI)
      {
        chr_received = SBUF;
        RI = 0;
        if (comm_state == MESSAGE_AWAITS_PROCESSING && !message_awaits_processing) comm_state = WAITING_FOR_TRAIN;
        switch(comm_state)
          {
            case WAITING_FOR_TRAIN:
            if(chr_received == TRAIN_CHR)
              {
                comm_state = RECEIVING_TRAIN;
                train_char_seen = 1;
                timeout_flag = TRUE;
              }
            break;

            case RECEIVING_TRAIN:
            if(!timeout_flag) comm_state = WAITING_FOR_TRAIN;

            if(chr_received == TRAIN_CHR)
              {
                train_char_seen++;
              }
            else if(chr_received != TRAIN_CHR && chr_received <= MAX_MESSAGE_LENGTH && train_char_seen >= TRAIN_LENGTH_RCV)
              {
                comm_state = RECEIVING_MESSAGE;
                message_buffer.content[0] = chr_received;
                message_buffer.index = 1;
              }
            else
              {
                comm_state = WAITING_FOR_TRAIN;
              }
            break;

            case RECEIVING_MESSAGE:
            if(!timeout_flag) comm_state = WAITING_FOR_TRAIN;

            message_buffer.content[message_buffer.index++] = chr_received;
            if (message_buffer.index == message_buffer.content[0])
              {
                comm_state = MESSAGE_AWAITS_PROCESSING;
                message_awaits_processing = TRUE;
                timeout_flag = FALSE;
              }
            break;

            case MESSAGE_AWAITS_PROCESSING:
            break;
          }
      }
    if (TI)
      {
        TI = 0;
        UART_busy = send_counter > 0;
        if (UART_busy)
          {
            send_counter--;
            SBUF = send_buffer [send_position++];
            if (send_position >= UART_XBUFLEN) send_position = 0;
          }
      }
  }

static unsigned char
reverse_bits(unsigned char byte)
{
  unsigned char reversed = byte; // r will be reversed bits of v; first get LSB of v
  unsigned char shift = 7; // extra shift needed at end

  for (byte >>= 1; byte; byte >>= 1)
    {
      reversed <<= 1;
      reversed |= byte & 1;
      shift--;
    }

  // shift when byte's highest bits are zero
  reversed <<= shift;
  return reversed;
}

// Reset serial communication
static void
reset_serial(void)
{
  // Disable serial communication
  ES = 0;

  // Setup the serial port operation mode
  // Reset receive and transmit interrupt flags and disable receiver enable
  RI = 0;
  TI = 0;
  REN = 0;
  // 8-bit UART mode for serial TIMER1 Mode2 SMOD=1
  SM1 = 1;
  SM0 = 0;

  // Set serial RxD and TxD lines to high
  P3_1 = 1;
  P3_0 = 1;

  // Multiprocessor communication disabled
  SM2 = 0;

  // Clear serial communication buffers
  send_counter = send_position = 0;
  UART_busy = FALSE;
  train_char_seen = 0;

  // Enable Serial interrupt and start listening on the bus
  ES = 1;
  REN = 1;
}

// Send a character to the UART
static void
UART_putc(unsigned char c)
{
  // Wait for room in buffer
  while (send_counter >= UART_XBUFLEN)
    ;
  ES = 0;
  if (UART_busy)
    {
      send_buffer[(unsigned char) (send_position + send_counter++)
          % UART_XBUFLEN] = c;
    }
  else
    {
      SBUF = c;
      UART_busy = 1;
    }
  ES = 1;
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
static unsigned int
calculate_CRC16(unsigned char *buf, unsigned char end_position)
{
  unsigned char i, c;
  unsigned int crc = 0xffff;
  unsigned char num;

  // Step through bytes in memory
  for (num = 0; num < end_position; num++)
    {
      // Flip the bits to comply with the true serial bit order
      c = reverse_bits(buf[num]);

      // Fetch byte from memory, XOR into  CRC top byte
      crc = crc ^ ((unsigned int) c << 8);

      // Prepare to rotate 8 bits
      for (i = 0; i < 8; i++)
        {
          // b15 is set...
          if (crc & 0x8000)
            // rotate and XOR with polynomial
            crc = (crc << 1) ^ CRC16_POLYNOMIAL;
          else
            // b15 is clear...
            // just rotate
            crc <<= 1;
        } // Loop for 8 bits
    } // Loop until num=0
  return crc; // Return updated CRC
}

/*
 * Public functions
 */

// Set the communication speed of the device
void
set_comm_speed(unsigned char speed)
{
  TR1 = 0; //Stop Timer 1
  TL1 = 0xff;    // Start from 255

  comm_speed = speed;

  // Set the reload value for the baud generator
  TH1 = comm_speeds[speed].reload_value;

  // Set the SMOD bit in PCON according to speed mode
  if (comm_speeds[speed].is_smod_set)
    PCON |= SMOD;
  else
    PCON &= 0x7F;

  // Set the timer  mode for the baud generator Timer1
  TMOD = (TMOD & 0x0f) | 0x20;    // Set Timer 1 Autoreload mode

  ET1 = 0;     // Disable timer1 interrupt to ensure smooth baud rate generation
  TR1 = 1;             // Start Timer 1
  reset_serial();       // Reset the communication
}

// Reset communication
void
reset_comm(void)
{
  // Clear message buffer
  message_buffer.index = 0;

  // Reset communication state
  comm_state = WAITING_FOR_TRAIN;
  msg_timeout_active = FALSE;
  timeout_flag = FALSE;
  message_awaits_processing = FALSE;

  // Clear receiving queue
  CRC_error_count = 0;

  reset_serial();
}

// Return the host address
unsigned char
get_host_address(void)
{
  return host_address;
}

// Set the host address
void
set_host_address(unsigned char _host_address)
{
  host_address = _host_address;
}

// Return the comm error
unsigned char
get_comm_error(void)
{
  return comm_error;
}

// Send a message on the serial line explicitely specifying SEQ
void
send_message_seq(unsigned char opcode, unsigned char seq)
{
  message_buffer.content[SEQ] = seq;
  send_message(opcode);
}

// Send a message on the serial line using the SEQ just received
void
send_message(unsigned char opcode)
{
  unsigned char i;
  unsigned int crc;
  message_buffer.content[LENGTH] = message_buffer.index + 3;
  message_buffer.content[OPCODE] = opcode;

  // Calculate the CRC
  crc = calculate_CRC16(message_buffer.content, message_buffer.index + CRC1);

  // Send the train sequence
  i = TRAIN_LENGTH_SND;
  while (i != 0)
    {
      UART_putc(TRAIN_CHR);
      i--;
    }

  // Send message body
  i = 0;
  while (i <= message_buffer.index + PARAMETER_END)
    {
      UART_putc(message_buffer.content[i]);
      i++;
    }

  UART_putc((unsigned char) ((crc & 0xff00) >> 8));
  UART_putc((unsigned char) (crc & 0x00ff));

  UART_putc(TRAIN_CHR);

  while (UART_busy)
    ;
}

// Periodically check if a message is received on the serial line
bool
get_message(void)
{

  // If there is no timeout running and reception in progress start watching timeout
  if (timeout_flag && !msg_timeout_active)
    {
      msg_timeout_active = TRUE;
      reset_timeout(MSG_TIMEOUT);
    }
  // Reset the UART if there was a timeout
  if (msg_timeout_active
      && timeout_occured(MSG_TIMEOUT, comm_speeds[comm_speed].msg_timeout))
    {
      // Safe to write as we are in timeout - nothing happens on the serial line
      comm_error = MESSAGING_TIMEOUT;
      message_buffer.index = 0;
      ES = 0;
      timeout_flag = FALSE;
      ES = 1;
      msg_timeout_active = FALSE;
      return FALSE;
    }

  // Return FALSE unless message received
  if (!message_awaits_processing)
    return FALSE;

  ES = 0;
  message_awaits_processing = FALSE;
  comm_state = WAITING_FOR_TRAIN;
  ES = 1;

  // OK, we have a structurrally correct message in the buffer
  msg_timeout_active = FALSE;
  message_buffer.index -= 3;

  // Check the CRC of the message
  if (calculate_CRC16(message_buffer.content, message_buffer.index + CRC1)
      != (unsigned int) ((message_buffer.content[message_buffer.index + CRC1]
          << 8) | (message_buffer.content[message_buffer.index + CRC2])))
    {
      // CRC is wrong: set error condition
      CRC_error_count++;
      comm_error = COMM_CRC_ERROR;
      message_buffer.index = 0;
      return FALSE;
    }
  else
    {
      comm_error = NO_ERROR;
      return TRUE;
    }
}

// Return the # of CRC errors seen
unsigned char
get_CRC_error_count(void)
{
  return CRC_error_count;
}

// Return the actual communication speed
unsigned char
get_comm_speed(void)
{
  return comm_speed;
}

// Return the messaging timeout value for the actual communication speed
unsigned char
get_messaging_timeout(void)
{
  return comm_speeds[comm_speed].msg_timeout;
}

// Return the response timeout value for the actual communication speed
unsigned char
get_response_timeout(void)
{
  return comm_speeds[comm_speed].resp_timeout;
}
