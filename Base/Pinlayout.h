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

// Watchdog pin
#define WATCHDOG_PIN P3_3

// Communication direction pins of the microcontroller
#define SLAVE_COMM_DIRECTION_PIN P1_7
#define MASTER_COMM_DIRECTION_PIN P1_0

// Define the pins of the SPI interface of the MCP4161
#define PIN_NCS P1_0
#define PIN_SCK P3_7
#define PIN_SDI_SDO P3_4


// Define on which port the onewire interface is
#define ONEWIRE_IS_ON_P3
//#define ONEWIRE_IS_ON_P1


/*
 * Extender switch specific pin assignments
 * For extender swich 74HC595, 8-bit parallell or serial out shift registers are used.
*/

#define DS_PIN P1_2
#define SHCP_PIN P1_3
#define NMR_PIN P1_4
#define STCP_PIN P1_5
#define NOE_PIN P1_6

#endif /* PINLAYOUT_H_ */
