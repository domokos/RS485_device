/*
 * Mast_con_test.c
 *
 *  Created on: Feb 19, 2013
 *      Author: dmolnar
 */

#include "Master_comm.h"

#define MASTER_HOST_ID 1


void main(void)
{
  EA = 1;
  init_master(MASTER_HOST_ID, COMM_SPEED_4800_H);
  operate_master();
}
