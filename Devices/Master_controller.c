/*
 * Master_controller.c
 *
 *  Created on: Feb 18, 2013
 *      Author: dmolnar
 */

#include "Master_controller.h"

#define MASTER_HOST_ID 1


void main(void)
{
  EA = 1;
  init_timer();

//  init_master(MASTER_HOST_ID, COMM_SPEED_4800_H);
  init_master(MASTER_HOST_ID, COMM_SPEED_115200_H);
  operate_master();
}
