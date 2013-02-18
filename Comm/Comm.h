/*
 * Comm.h
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#ifndef COMM_H_
#define COMM_H_

#include "Base.h"
#include "Comm_common.h"

// Communication parameters
#define MAX_MESSAGE_LENGTH 15
#define TRAIN_LENGTH_RCV 8
#define TRAIN_LENGTH_SND 20

// Messaging states
#define WAITING_FOR_TRAIN 0
#define RECEIVING_TRAIN 1
#define IN_SYNC 2
#define RECEIVING_MESSAGE 3

/*
 * The number after which get_message is called while waiting for a character
 * during transmission of a frame but receiving none that will cause
 * it to reset: a timeout limit.
*/
#define MESSAGE_TIMEOUT_COUNT_LIMIT 500

// Messaging error conditions
#define NO_ERROR 0 // No error
#define NO_TRAIN_RECEIVED 1 // Expected train sequence, got something else => Ignoring communication
#define MESSAGE_TOO_LONG 2 // Receive buffer length exceeded
#define MESSAGING_TIMEOUT 3 // Timeout occured - expected but no communication is seen on the bus
#define COMM_CRC_ERROR 4 // Frame with CRC error received


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


// Redefine message position macros
#ifdef PARAMETER_END
#undef PARAMETER_END
#define PARAMETER_END message_buffer.index
#endif

#ifdef CRC1
#undef CRC1
#define CRC1 message_buffer.index+1
#endif

#ifdef CRC2
#undef CRC2
#define CRC2 message_buffer.index+2
#endif


/*
 * Variable definitions of the module
 */

extern static unsigned char train_length;
extern static unsigned char comm_error;
extern static unsigned char comm_state;
extern static unsigned char CRC_burst_error_count;
extern static unsigned int message_timeout_counter;
extern static struct message_struct message_buffer;


/*
 * Public function library
 */

// Function to send response to the master on the bus
void send_response(unsigned char opcode, unsigned char seq);

// Provide access to the message structure
struct message_struct* get_message_buffer(void);

// Get the error state of the comm module
unsigned char get_comm_error(void);

// Return the # of CRC errors seen
unsigned char get_CRC_burst_error_count(void);

// Return the state of the communication
unsigned char get_comm_state(void);

// Return train length seen
unsigned char get_train_length(void);

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

// Set the direction of communication
static void set_comm_direction(unsigned char direction);

static void count_and_perform_timeout(void);

#endif /* COMM_H_ */
