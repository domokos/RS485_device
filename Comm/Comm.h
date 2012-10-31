/*
 * Comm.h
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#ifndef COMM_H_
#define COMM_H_

#include <at89x051.h>
#include "Base.h"

#define MAX_MESSAGE_LENGTH 15

// Messaging buffer STRUCT
struct message_struct
{
  unsigned char   content[MAX_MESSAGE_LENGTH];
  unsigned char   index;
};

/*
 * The messaging format is:
 * START_FRAME - 8 bits
 * SLAVE_ADDRESS - 8 bits
 * SEQ - 8 bits
 * COMMAND - 8 bits
 * PARAMERER - arbitrary number of bytes
 * CRC - 8 bits calculated for the data between start and end frame
 * END_FRAME - 8 bits
 *
 *  * The SEQ field holds a message sequence number
 *      SEQ
 */

// The buffer indexes
#define SLAVE_ADDRESS 1
#define SEQ 2
#define OPCODE 3
#define PARAMETER_START 3
#define CRC message_buffer.index
#define PARAMETER_END message_buffer.index-1

// The ISR prototypes to be included in the main program;
void Serial_ISR(void)  __interrupt 4 __using 0;

// Initialize comm module
void init_comm(unsigned char _host_address);

// Must be called periodically to allow comm module to perform housekeeping
// Returns void* to the caller if no message is received
// returns a pointer to the message if a message is received
struct message_struct* get_message(void);

// Function to send response to the master on the bus
void send_response(unsigned char opcode);

// Provide access to the message structure
struct message_struct* get_message_buffer(void);

/*
Internal functions not to be accessed from outside of the comm modul

//Check 8-Bit checksum with given polynomial
unsigned char calculate_message_CRC();

// Send ACK response to a message
void ack_message(bool success);

// Send a character to the UART
void UART_putchar(unsigned char value);

*/

// Messaging states
#define AWAITING_START_FRAME 0
#define RECEIVING_MESSAGE 1
#define POSTPROCESSING_MESSAGE 2

// Messaging frame structure elements
#define START_FRAME 0x55
#define END_FRAME 0x5d
#define MESSAGE_ESCAPE 0x7d

// Messaging error conditions
#define NO_ERROR 0 // No error
#define NO_START_FRAME_RECEIVED 1 // Expected message start frame, got something else => Ignoring the frame
#define MESSAGE_TOO_LONG 2 // Recieve buffer length exceeded

// CRC generator polynomial
#define CRC_POLYNOMIAL 0x131


// Response opcodes
#define CRC_ERROR 0 // The message contained CRC error


#endif /* COMM_H_ */
