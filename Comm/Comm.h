/*
 * Comm.h
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#ifndef COMM_H_
#define COMM_H_

#include "Base.h"

#define MAX_MESSAGE_LENGTH 15
#define TRAIN_LENGTH_RCV 8
#define TRAIN_LENGTH_SND 20

// Messaging states
#define WAITING_FOR_TRAIN 0
#define RECEIVING_TRAIN 1
#define IN_SYNC 2
#define RECEIVING_MESSAGE 3
#define RECEIVING_CRC1 4
#define RECEIVING_CRC2 5

/*
 * The number after which get_message is called while waiting for a character
 * during transmission of a frame but receiving none that will cause
 * it to reset: a timeout limit.
*/
#define MESSAGE_TIMEOUT_COUNT_LIMIT 500

// Messaging frame structure elements
#define TRAIN_CHR 0xff
#define ESCAPE_CHR 0x7d

// Messaging error conditions
#define NO_ERROR 0 // No error
#define NO_TRAIN_RECEIVED 1 // Expected train sequence, got something else => Ignoring communication
#define MESSAGE_TOO_LONG 2 // Receive buffer length exceeded
#define MESSAGING_TIMEOUT 3 // Timeout occured - expected but no communication is seen on the bus
#define COMM_CRC_ERROR 4 // Frame with CRC error received

// CRC generator polynomial
#define CRC16_POLYNOMIAL 0x1021

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


// Messaging buffer STRUCT
struct message_struct
{
  unsigned char   content[MAX_MESSAGE_LENGTH];
  unsigned char   index;
};


/*
 * Pins of the microcontroller reserved for communication
*/

#define COMM_DIRECTION_PIN P3_7
// States of the communication direction pin
#define DEVICE_LISTENS 0
#define DEVICE_SENDS 1

/**********************************************************************************
 * The messaging format:
 * TRAIN_CHR - n*8 bits
 * SLAVE_ADDRESS - 8 bits
 * SEQ - 8 bits
 * OPCODE - 8 bits
 * PARAMERER - arbitrary number of bytes
 * TRAIN_CHR - indicating end of message
 * CRC - 2*8 bits calculated for the data excluding start frame
 * Train_CHR - 8 bits - to make sure bus state remains in send during transmitting CRC
 *
 *  * The SEQ field holds a message sequence number
 *  * Index of the message buffer points to the last parameter byte
 ***********************************************************************************/

// The buffer indexes
#define SLAVE_ADDRESS 0
#define SEQ 1
#define OPCODE 2
#define PARAMETER_START 3
#define CRC1 message_buffer.index+1
#define CRC2 message_buffer.index+2
#define PARAMETER_END message_buffer.index

// The ISR prototypes to be included in the main program;
ISR(SERIAL,0);

/*
 * Public function library
 */

// Function to send response to the master on the bus
void send_response(unsigned char opcode, unsigned char seq);

// Provide access to the message structure
struct message_struct* get_message_buffer(void);

// Get the error state of the comm module
unsigned char get_comm_error(void);

// Indicate if UART has lost a char
bool is_UART_char_lost(void);

// Return the host address
unsigned char get_host_address(void);

// Set the host address
void set_host_address(unsigned char _host_address);

// Return the # of CRC errors seen
unsigned char get_CRC_burst_error_count(void);

// Return the state of the communication
unsigned char get_comm_state(void);

// Reset the state of the communication
void reset_comm(void);

// Initialize comm module
void init_comm(unsigned char host_address, unsigned char comm_speed);

// Set communication speed
void set_comm_speed(unsigned char comm_speed);

// Must be called periodically to allow comm module to perform housekeeping
// Returns void* to the caller if no message is received
// returns a pointer to the message if a message is received
struct message_struct* get_message(void);

// Low level flood test of the bus
void bus_flood_test(unsigned char character, int repeat);

/*
 * Internal functions - should not be called from outside of the comm modul
*/

//Calculate the 16-Bit checksum of the message
static unsigned char calculate_message_CRC();

// Send a character to the UART
static void UART_putchar(unsigned char value);

// Set the direction of communication
static void set_comm_direction(unsigned char direction);

static void count_and_perform_timeout(void);

#endif /* COMM_H_ */
