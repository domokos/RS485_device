/*
 * Comm.h
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#ifndef COMM_H_
#define COMM_H_

#include "Base.h"

// Initialize comm module
void init_comm(void);

// Must be called periodically to allow comm module to perform housekeeping
// Returns false to the caller if no further attention is needed and
// returns true if a message is received
bool operate_comm(void);

// Messaging buffer STRUCT
struct buffer_struct
{
	unsigned char 	content[15];
	unsigned char 	index;
};

// Messaging states
#define AWAITING_TRAIN_SEQ 0
#define RECEIVING_TRAIN_SEQ 1
#define AWAITING_MESSAGE_HEAD 2
#define RECEIVING_MESSAGE 3
#define MESSAGE_RECEIVED 4

#define TRAIN_CHAR 0x55
#define OUT_OF_SYNC_TRAIN_CHAR 0xaa

#endif /* COMM_H_ */
des
