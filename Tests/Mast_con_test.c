/*
 * Mast_con_test.c
 *
 *  Created on: Feb 19, 2013
 *      Author: dmolnar
 */

#include "Master_comm.h"

#define MASTER_HOST_ID 0

struct message_struct *MSG_buffer;

void main(void)
{
  init_master(MASTER_HOST_ID, COMM_SPEED_4800_H);
  operate_master();
}
