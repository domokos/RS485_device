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

// Messaging buffer STRUCT
struct message_struct
{
        unsigned char   content[15];
        unsigned char   index;
        unsigned char   address;
        unsigned char   seq;
};



// Initialize comm module
void init_comm(unsigned char _host_address);

// Must be called periodically to allow comm module to perform housekeeping
// Returns false to the caller if no further attention is needed and
// returns true if a message is received
bool operate_comm(void);

// Provide access to the message structure
struct message_struct* get_message();

/*
Internal functions not to be accessed from outside of the comm modul

//Check 8-Bit checksum with given polynomial
unsigned char calculate_message_CRC();

// Send ACK response to a message
void ack_message(bool success);

*/

/*
 * The messaging format is:
 * START_FRAME - 8 bits
 * ADDRESS - 8 bits
 * SEQ - 8 bits
 * COMMAND - 8 bits
 * PARAMERER - arbitrary number of bits
 * CRC - 8 bits
 * END_FRAME - 8 bits
 *
 * The ADDRESS field structure is as follows:
 *      low bits 0-3:  Sender address
 *      high bits 4-7: Reciever address
 *
 * The CONTROL field is as follows:
 *      SEQ
 *
 *
 *
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
#define CRC_POLYNOMIAL 0x131

#endif /* COMM_H_ */
