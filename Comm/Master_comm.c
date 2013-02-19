/*
 * Master_comm.c
 *
 *  Created on: Feb 19, 2013
 *      Author: dmolnar
 */


#include "Base.h"
#include "Master_comm.h"


static unsigned char train_length;
static unsigned char comm_error;
static unsigned char comm_state;
static unsigned char CRC_burst_error_count;
static unsigned int message_timeout_counter;

