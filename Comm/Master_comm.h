/*
 * Master_comm.h
 *
 *  Created on: Feb 19, 2013
 *      Author: dmolnar
 */

#ifndef MASTER_COMM_H_
#define MASTER_COMM_H_


// Master communication states
#define MASTER_COMM_DIRECTION_PIN P1_0
#define MASTER_TALKS_TO_HOST 0
#define MASTER_TALKS_ON_BUS 1


// Master communication states on the bus
#define MASTER_SENDS_ON_BUS 1
#define MASTER_LISTENS_ON_BUS 0
#define MASTER_BUS_COMM_DIRECTION_PIN P3_7



#endif /* MASTER_COMM_H_ */
