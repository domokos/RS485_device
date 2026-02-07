/*
 * Fancoil_device.c
 *
 *  Created on: Jan 4, 2026
 *      Author: doma
 */


#include "Generic_device.h"
#include "Fancoil_device.h"

/*
 * Define registers of this device
 */
// This device has 6 registers
__code const unsigned char nr_of_registers = 6;

// Describe the registers of this device
__code const unsigned char register_identification[][REG_IDENTIFICATION_LEN] =
  {
      // Relay output - Contact Low Tariff
        { REG_TYPE_SW, REG_RW, 2, DONT_CARE, DONT_CARE },
      // Relay output - Contact Night Mode
        { REG_TYPE_SW, REG_RW, 2, DONT_CARE, DONT_CARE },
      // Relay output - Contact Heat/Cool Mode
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Relay output - Contact Dual Setpoint
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE }};

/* Device entry point */
void
main(void)
{
    // Enable interrupts and initialize timer
    EA = 1;
    init_timer();

    // Set 9600 baud
    init_device_comm(HOST_ID, COMM_SPEED_9600_H);

    test_potmeter();

}

void
test_potmeter(void)
{
  unsigned int value = 0;
  unsigned int sleep = 0;
  bool up = TRUE;

  reset_potentiometer();
  while(TRUE){
      sleep+=10;
      PIN_FC_ACTIVE = sleep > 0x7fff ? 0 : 1;

      write_wiper(value, TRUE, WIPER_HW);
      delay_msec(2);
      if(up){
	  if(value < 210) {
	      value+=10;
	  } else {
	      up = FALSE;
	      value=220;
	  }
      } else {
	  if(value > 10) {
	      value-=10;
	  } else {
	      up = TRUE;
	      value=0;
	  }
      }
  }
}
