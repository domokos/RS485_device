/*
 * Comm_common.h
 *
 *  Created on: Feb 18, 2013
 *      Author: dmolnar
 */

#include "Base.h"

#ifndef COMM_COMMON_H_
#define COMM_COMMON_H_

// Serial buffer sizes
#define XBUFLEN 4
#define RBUFLEN 8


// Messaging frame structure elements
#define TRAIN_CHR 0xff
#define ESCAPE_CHR 0x7d

// CRC generator polynomial
#define CRC16_POLYNOMIAL 0x1021


/**********************************************************************************
 * The messaging format:
 * TRAIN_CHR - n*8 bits - at least TRAIN_LENGTH_RCV
 * LENGTH - the length of the message
 * SLAVE_ADDRESS - 8 bits
 * SEQ - 8 bits
 * OPCODE - 8 bits
 * PARAMERER - arbitrary number of bytes
 * CRC - 2*8 bits calculated for the data excluding start frame
 *
 *  * The SEQ field holds a message sequence number
 *  * Index of the message buffer points to the last parameter byte
 ***********************************************************************************/

// The message indexes
#define LENGTH 0
#define SLAVE_ADDRESS 1
#define SEQ 2
#define OPCODE 3
#define PARAMETER_START 4
#define PARAMETER_END message_end
#define CRC1 message_end+1
#define CRC2 message_buffer.index+2


/*
 * Command opcodes
 */

// Set the value of a register
#define SET_REGISTER 0
// Read the value of a register
#define READ_REGISTER 1
// Identify a register by returning its description
#define IDENTTIFY_REGISTER 2
// Reset the device to its basic state
#define RESET_DEVICE 3
// Perform tests
#define COMM_TEST_REVERSE_MESSAGE 4
// PING - master expects an echo and the same payload
#define PING 5
// Set communication speed
#define SET_COMM_SPEED 6


/*
 * COMMAND PARAMETERS
 */
// Parameters of SET_COMM_SPEED
// Timer1 reload and SMOD bit PCON values for 11.0592 MHz Crystal
struct comm_speed_struct
{
  unsigned char   reload_value;
  unsigned char   is_smod_set;
};

#define COMM_SPEED_300_L 0
#define COMM_SPEED_1200_L 1
#define COMM_SPEED_2400_L 2
#define COMM_SPEED_4800_L 3
#define COMM_SPEED_9600_L 4
#define COMM_SPEED_14400_L 5
#define COMM_SPEED_28800_L 6
#define COMM_SPEED_300_H 7
#define COMM_SPEED_1200_H 8
#define COMM_SPEED_2400_H 9
#define COMM_SPEED_4800_H 10
#define COMM_SPEED_9600_H 11
#define COMM_SPEED_14400_H 12
#define COMM_SPEED_19200_H 13
#define COMM_SPEED_28800_H 14
#define COMM_SPEED_57600_H 15


/*
 *  Response opcodes
 */

// The received message contained CRC error
// The message has a zero length payload. CRC follows the opcode
#define CRC_ERROR 0
// Command succesfully received response messge payload
// contains the information requested by the master
#define COMMAND_SUCCESS 1
// Command succesfully received, execution of the
// requested operation failed, original status preserved or
// status undefined
#define COMMAND_FAIL 2
// Response to a PING message - should contain the same
// message received in the PING
#define ECHO 3


/*
 * Internal utility functions
 */

// The ISR prototypes to be included in the main program;
ISR(SERIAL,0);

// Flip the bits in a byte
static unsigned char flip_bits(unsigned char byte);

/*
 * Public utility functions
 */

//Calculate the 16-Bit checksum of the message
unsigned int calculate_CRC16(unsigned char *buf, unsigned char end_position);

// Set the communication speed of the device
void set_comm_speed(unsigned char comm_speed);

// Reset serial communication
void reset_serial(void);

// Send a character to the UART
void UART_putc(unsigned char c);

// Read a character from the UART buffer
unsigned char UART_getc(void);

// Are there any caharcters in the UART buffer available for reading?
unsigned char UART_is_char_available(void);

// Is UART character transmission complete?
char is_UART_send_complete (void);

// Return the host address
unsigned char get_host_address(void);

// Set the host address
void set_host_address(unsigned char _host_address);


#endif /* COMM_COMMON_H_ */
