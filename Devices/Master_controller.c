/*
 * Master_controller.c
 *
 *  Created on: Feb 18, 2013
 *      Author: dmolnar
 */

#include "Master_controller.h"

void main(void)
{
  EA = 1;
  init_timer();

  init_master(MASTER_HOST_ID, COMM_SPEED_9600_H);
  operate_master();
}
