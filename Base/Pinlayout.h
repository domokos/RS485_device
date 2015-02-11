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
#ifdef __CDT_PARSER__
  #define WATCHDOG_PIN
  #define COMM_DIRECTION_PIN
  // Define on which port the onewire interface is
  #define ONEWIRE_IS_ON_P1
  #define HEATER_RELAY_PIN // Heater contact

#elif defined MASTER_CONTROLLER_DEVICE
  #define WATCHDOG_PIN P3_5
  #define COMM_DIRECTION_PIN P1_0

#elif defined MAIN_PANEL_SLAVE_DEVICE
  #define WATCHDOG_PIN P3_3
  #define COMM_DIRECTION_PIN P1_7
  // Define on which port the onewire interface is
  #define ONEWIRE_IS_ON_P3
  #define HEATER_RELAY_PIN P3_5 // Heater contact

#elif defined SIX_OWBUS_SLAVE_DEVICE
  #define WATCHDOG_PIN P3_5
  #define COMM_DIRECTION_PIN P3_7
  // Define on which port the onewire interface is
  #define ONEWIRE_IS_ON_P1

#elif defined MIXER_CONTROL_SLAVE_DEVICE
  #define WATCHDOG_PIN P3_5
  #define COMM_DIRECTION_PIN P3_7
  // Define on which port the onewire interface is
  #define ONEWIRE_IS_ON_P1

#else
  #error "Basic pinlayout definitions could be determined"
#endif

#ifdef __CDT_PARSER__
  // Define the pins of the SPI interface of the MCP4161
  #define PIN_NCS_HW
  #define PIN_NCS_HEAT
  #define PIN_SCK
  #define PIN_SDI_SDO

  // Define the pins of the Extender interface of the MCP4161
  #define DS_PIN
  #define NOE_PIN
  #define STCP_PIN
  #define SHCP_PIN
  #define NMR_PIN

  // Define pins of the mixer controller
  #define CCW_PIN
  #define CW_PIN
  #define SPAREA_PIN
  #define SPAREB_PIN
  #define GPIO_PIN

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
#elif defined MIXER_CONTROL_SLAVE_DEVICE

  // Define pins of the mixer controller
  #define CCW_PIN P1_5
  #define CW_PIN P1_6
  #define SPAREA_PIN P1_3
  #define SPAREB_PIN P1_4
  #define GPIO_PIN P3_7

#else
  #error "Type specific pinlayout definitions cannot be determined"
#endif

#endif /* PINLAYOUT_H_ */
