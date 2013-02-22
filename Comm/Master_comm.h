/*
 * Master_comm.h
 *
 *  Created on: Feb 19, 2013
 *      Author: dmolnar
 */

#include "Base.h"
#include "Comm_common.h"

#ifndef MASTER_COMM_H_
#define MASTER_COMM_H_


// Master communication states
#define MASTER_COMM_DIRECTION_PIN P1_0
#define MASTER_TALKS_TO_HOST 0
#define MASTER_TALKS_TO_BUS 1

// Master communication states on the bus
#define MASTER_SENDS_ON_BUS 1
#define MASTER_LISTENS_ON_BUS 0
#define MASTER_BUS_COMM_DIRECTION_PIN P3_7

// Master communication state machine states
#define SM_MASTER_LISTENS_TO_HOST 0
#define SM_MASTER_LISTENS_ON_BUS 1



/*
 * Internal Utility function prototypes
 */

// Set the direction of communication
static void set_master_comm_state(unsigned char direction);

// Set master's communication direction when communicating on the bus
static void set_master_bus_comm_direction(unsigned char direction);


/*
 * Public module functions
 */

// Reset the state of the communication
void reset_master_comm(void);

// Initialize the master
void init_master(unsigned char host_address, unsigned char comm_speed);

// Operate the master device - never returns
// Facilitates bus master operation
void operate_master(void);

#endif /* MASTER_COMM_H_ */
