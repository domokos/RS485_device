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

/*
 * The number after which get_message is called while waiting for a character
 * during transmission of a frame but receiving none that will cause
 * it to reset: a timeout limit.
*/
#define MESSAGE_TIMEOUT_COUNT_LIMIT 500


/*
 * Pins of the microcontroller reserved for communication
*/

#define COMM_DIRECTION_PIN P3_7
// States of the communication direction pin
#define DEVICE_LISTENS 0
#define DEVICE_SENDS 1


/*
 * Public function library
 */

// Function to send response to the master on the bus
void send_response(unsigned char opcode, unsigned char seq);

// Reset the state of the communication
void reset_device_comm(void);

// Initialize comm module
void init_device_comm(unsigned char host_address, unsigned char comm_speed);

// Set communication speed
void set_comm_speed(unsigned char comm_speed);

// Must be called periodically to allow comm module to perform housekeeping
// Returns void* to the caller if no message is received
// returns a pointer to the message if a message is received
struct message_struct* get_device_message(void);

/*
 * Internal functions - should not be called from outside of the comm modul
*/

// Set the direction of communication
static void set_comm_direction(unsigned char direction);

#endif /* COMM_H_ */
