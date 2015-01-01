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

#ifdef MASTER_CONTROLLER_DEVICE
  #define WATCHDOG_PIN P3_5
  #define COMM_DIRECTION_PIN P1_0
#elif defined MAIN_PANEL_SLAVE_DEVICE
  #define WATCHDOG_PIN P3_3
  #define COMM_DIRECTION_PIN P1_7
  // Define on which port the onewire interface is
  #define ONEWIRE_IS_ON_P3
#elif defined SIX_OWBUS_SLAVE_DEVICE
  #define WATCHDOG_PIN P3_5
  #define COMM_DIRECTION_PIN P1_7
  // Define on which port the onewire interface is
  #define ONEWIRE_IS_ON_P1
#else
  #error "No comm direction pin could be determined"
#endif

#ifdef __CDT_PARSER__0
  // Define the pins of the SPI interface of the MCP4161
  #define PIN_NCS_HW P1_0
  #define PIN_NCS_HEAT P1_1
  #define PIN_SCK P3_7
  #define PIN_SDI_SDO P3_4

  // Define the pins of the Extender interface of the MCP4161
  #define DS_PIN P1_2
  #define NOE_PIN P1_3
  #define STCP_PIN P1_4
  #define SHCP_PIN P1_5
  #define NMR_PIN P1_6

#elif defined MAIN_PANEL_SLAVE_DEVICE
  // Define the pins of the SPI interface of the MCP4161
  #define PIN_NCS_HW P1_0
  #define PIN_NCS_HEAT P1_1
  #define PIN_SCK P3_7
  #define PIN_SDI_SDO P3_4

  // Define the pins of the Extender interface of the MCP4161
  #define DS_PIN P1_2
  #define NOE_PIN P1_3
  #define STCP_PIN P1_4
  #define SHCP_PIN P1_5
  #define NMR_PIN P1_6
#endif

#endif /* PINLAYOUT_H_ */
