/*
 * Pinlayout.h
 *
 *  Created on: Nov 25, 2014
 *      Author: dmolnar
 */

#ifndef PINLAYOUT_H_
#define PINLAYOUT_H_

/*
 *  This section defines the pins assignments the microcontroller
 */

/* Watchdog pin
 * This - for PCB layoutting reasons - is:
 *
 *  P3_5 on the Master Controller
 *  P3_5 on the Six Owbus Device
 *  P3_3 on the Main panel device
 */

#define WATCHDOG_PIN P3_3

// Communication direction pins of the microcontroller
#define SLAVE_COMM_DIRECTION_PIN P1_7
#define MASTER_COMM_DIRECTION_PIN P1_0

// Define the pins of the SPI interface of the MCP4161
#define PIN_NCS_HW P1_0
#define PIN_NCS_HEAT P1_1
#define PIN_SCK P3_7
#define PIN_SDI_SDO P3_4

// Define on which port the onewire interface is
#define ONEWIRE_IS_ON_P3
//#define ONEWIRE_IS_ON_P1

#define DS_PIN P1_2
#define NOE_PIN P1_3
#define STCP_PIN P1_4
#define SHCP_PIN P1_5
#define NMR_PIN P1_6

#endif /* PINLAYOUT_H_ */
