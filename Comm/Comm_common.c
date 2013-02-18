/*
 * Comm_common.c
 *
 *  Created on: Feb 18, 2013
 *      Author: dmolnar
 */

#include "Comm_common.h"


// Global variables facilitating communication

static unsigned char rcv_buffer[RBUFLEN], send_buffer[XBUFLEN];
static unsigned char rcv_counter, send_counter, rcv_position, send_position;
static bool UART_busy;
static unsigned char host_address;

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
	 return(crc); // Return updated CRC
}

// Send a character to the UART
void UART_putc(unsigned char c)
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
unsigned char UART_getc(void)
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
unsigned char UART_is_char_available(void)
{
   return rcv_counter;
}

// Is UART character transmission complete?
char is_UART_send_complete (void)
{
   return XBUFLEN - send_counter;
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
