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

// Initialize comm module
void init_comm(unsigned char _host_address);

// Must be called periodically to allow comm module to perform housekeeping
// Returns false to the caller if no further attention is needed and
// returns true if a message is received
bool operate_comm(void);

/*
 * The messaging format is:
 * START_FRAME - 8 bits
 * ADDRESS - 8 bits
 * CONTROL - 8 bits
 * PAYLOAD - arbitrary number of bits
 * CRC - 8 bits
 * END_FRAME - 8 bits
 *
 * The CONTROL field is as follows:
 *
 *
 *
 */


// Messaging buffer STRUCT
struct buffer_struct
{
	unsigned char 	content[15];
	unsigned char 	index;
};

// Messaging states
#define AWAITING_START_FRAME 0
#define AWAITING_ADDRESS 1
#define RECEIVING_MESSAGE 2
#define MESSAGE_RECEIVED 3

// Messaging frame structure elements
#define START_FRAME 0x55
#define END_FRAME 0x5d
#define MESSAGE_ESCAPE 0x7d

// Messaging error conditions
#define NO_ERROR 0 // No error
#define NO_START_FRAME_RECEIVED 1 // Expected message start frame, got something else => Ignoring the frame

#endif /* COMM_H_ */
