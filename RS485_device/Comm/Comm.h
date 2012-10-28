/*
 * Comm.h
 *
 *  Created on: Oct 22, 2012
 *      Author: dmolnar
 */

#ifndef COMM_H_
#define COMM_H_

void init_comm();

// Messaging buffer STRUCT
struct buffer_struct
{
	unsigned char 	content[15];
	unsigned char 	index;
};

// Messaging states
#define AWAITING_TRAIN_SEQ 0
#define RECEIVIN_TRAIN_SEQ 1
#define AWAITING_MESSAGE 2
#define RECEIVING_MESSAGE 3
#define MESSAGE_RECEIVED 4



#endif /* COMM_H_ */
