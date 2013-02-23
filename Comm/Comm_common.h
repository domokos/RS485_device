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


// Communication parameters
#define MAX_MESSAGE_LENGTH 15
#define TRAIN_LENGTH_RCV 8
#define TRAIN_LENGTH_SND 20

// Messaging states
#define WAITING_FOR_TRAIN 0
#define RECEIVING_TRAIN 1
#define IN_SYNC 2
#define RECEIVING_MESSAGE 3

// Messaging error conditions
#define NO_ERROR 0 // No error
#define NO_TRAIN_RECEIVED 1 // Expected train sequence, got something else => Ignoring communication
#define MESSAGE_TOO_LONG 2 // Receive buffer length exceeded
#define MESSAGING_TIMEOUT 3 // Timeout occured - expected but no communication is seen on the bus
#define COMM_CRC_ERROR 4 // Frame with CRC error received

// Messaging timeout types
#define MSG_TIMEOUT 0
#define RSP_TIMEOUT 1

// Messaging buffer STRUCT
struct message_struct
{
  unsigned char   index;
  unsigned char   content[MAX_MESSAGE_LENGTH];
};

/**********************************************************************************
 * The messaging format:
 * TRAIN_CHR - n*8 bits - at least TRAIN_LENGTH_RCV
 * LENGTH - the length of the message
 * MASTER_ADDRESS - 8 bits the address of the master
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
#define MASTER_ADDRESS 1
#define SLAVE_ADDRESS 2
#define SEQ 3
#define OPCODE 4
#define PARAMETER_START 5
#define PARAMETER_END 0 // message_end - index+0
#define CRC1 1 // - index+1
#define CRC2 2 // - index+2


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
// Timer1 reload, SMOD bit PCON values for 11.0592 MHz Crystal and
// messaging timeout in baud generator timer interrupt count -
//  for host master message exchange timeout 4 times the value needs to be used

struct comm_speed_struct
{
  unsigned char   reload_value;
  unsigned char   is_smod_set;
  unsigned int    msg_timeout;
  unsigned int    resp_timeout;
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


extern const struct comm_speed_struct comm_speeds[];



/*
 *  Response opcodes
 */

// This must hold values to workaround SDCC bug of trying to call lptrput
extern const unsigned char response_opcodes[];

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
// timaout report to the host by the master
#define TIMEOUT 4


/*
 * Internal utility functions
 */

// The ISR prototypes to be included in the main program;
ISR(SERIAL,0);

// Flip the bits in a byte
static unsigned char flip_bits(unsigned char byte);

// Reset serial communication
static void reset_serial(void);

// Send a character to the UART
static void UART_putc(unsigned char c);

// Read a character from the UART buffer
static unsigned char UART_getc(void);

// Are there any caharcters in the UART buffer available for reading?
static unsigned char UART_is_char_available(void);

// Is UART character transmission complete?
static char is_UART_send_complete (void);

//Calculate the 16-Bit checksum of the message
static unsigned int calculate_CRC16(unsigned char *buf, unsigned char end_position);


// Handle timeout events
static unsigned char evaluate_timeout();

/*
 * Public utility functions
 */

// Set the communication speed of the device
void set_comm_speed(unsigned char comm_speed);

// Return the currently set communication speed
unsigned char get_comm_speed(void);

// Reset communication
void reset_comm(void);

// Return the host address
unsigned char get_host_address(void);

// Set the host address
void set_host_address(unsigned char _host_address);

// Provide access to the message structure
struct message_struct* get_message_buffer(void);

// Get the error state of the comm module
unsigned char get_comm_error(void);

// Send a message on the seria line
void send_message(unsigned char opcode, unsigned char seq);

// Periodically listen for/get a message on the serial line
struct message_struct* get_message(void);

// Return the # of CRC errors seen
unsigned char get_CRC_burst_error_count(void);

// Return the state of the communication
unsigned char get_comm_state(void);

// Return the actual communication speed
unsigned char get_comm_speed(void);

// Return the messaging timeout value for the actual communication speed
unsigned char get_messaging_timeout(void);

// Return the response timeout value for the actual communication speed
unsigned char get_response_timeout(void);

#endif /* COMM_COMMON_H_ */
