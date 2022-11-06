/*
 * Pinlayout.h
 *
 *  Created on: Nov 25, 2014
 *      Author: dmolnar
 */

#ifndef PINLAYOUT_H_
#define PINLAYOUT_H_

/*
 *  This section defines the pins assignments of the microcontroller
 */
#ifdef __CDT_PARSER__

  #define MASTER_HOST_ID
  #define HOST_ID

  #define WATCHDOG_PIN
  #define COMM_DIRECTION_PIN
  // Define on which port the onewire interface is
  #define ONEWIRE_IS_ON_P1

#elif defined MASTER_CONTROLLER_DEVICE
  // Define the ID of this master
  #define MASTER_HOST_ID 1

  #define WATCHDOG_PIN P3_5
  #define COMM_DIRECTION_PIN P1_0

#elif defined MAIN_PANEL_SLAVE_DEVICE
  // The id of this host on the bus
  #define HOST_ID 11

  #define WATCHDOG_PIN P3_3
  #define COMM_DIRECTION_PIN P1_7
  // Define on which port the onewire interface is
  #define ONEWIRE_IS_ON_P3
  #define HEATER_RELAY_PIN P3_5 // Heater contact

#elif defined SIX_OWBUS_SLAVE_DEVICE
  // The id of this host on the bus
  #define HOST_ID 10

  #define WATCHDOG_PIN P3_5
  #define COMM_DIRECTION_PIN P3_7
  // Define on which port the onewire interface is
  #define ONEWIRE_IS_ON_P1

#elif defined MIXER_CONTROL_SLAVE_DEVICE
  // The id of this host on the bus
  #define HOST_ID 12

  #define WATCHDOG_PIN P3_3
  #define COMM_DIRECTION_PIN P1_7
  // Define on which port the onewire interface is
  #define ONEWIRE_IS_ON_P1

#elif defined HP_SLAVE_DEVICE
  // The id of this host on the bus
  #define HOST_ID 13

  #define COMM_DIRECTION_PIN P3_2
  #define EHS_PIN P3_7
  #define MODE_PIN P3_5
  #define BACKUP_PIN P3_4
  #define ALARM_PIN P3_3


#else
  #error "Basic pinlayout definitions could be determined"
#endif

#ifdef __CDT_PARSER__
  // Define the pins of the SPI interface of the MCP4161
  #define PIN_NCS_HW
  #define PIN_NCS_HEAT
  #define PIN_SCK
  #define PIN_SDI_SDO

  // Main panel heater pin
  #define HEATER_RELAY_PIN

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
  #define USEBUFFER_PIN
  #define GPIO_PIN

  // Define pins of the HP device
  #define EHS_PIN
  #define MODE_PIN
  #define BACKUP_PIN
  #define ALARM_PIN
  #define PIN_SERDATA
  #define PIN_SDO
  #define PIN_NCS_CSEL

#elif defined MAIN_PANEL_SLAVE_DEVICE
  // Define the pins of the SPI interface of the MCP4161
  #define PIN_NCS_HW P1_0
  #define PIN_NCS_HEAT P1_1
  #define PIN_SCK P3_7
  #define PIN_SDI_SDO P3_4

  // Heater contact
  #define HEATER_RELAY_PIN P3_5

  // Define the pins of the Extender interface
  #define DS_PIN P1_2
  #define NOE_PIN P1_3
  #define STCP_PIN P1_4
  #define SHCP_PIN P1_5
  #define NMR_PIN P1_6

#elif defined HP_SLAVE_DEVICE

  #define PIN_SERDATA P1_3

  // Define the pins of the SPI interface of the MCP416x
  #define PIN_NCS_CSEL P1_0
  #define PIN_SCK P1_1
  #define PIN_SDO P1_2

  // Define the pins of the Extender interface
  #define NOE_PIN P1_4
  #define STCP_PIN P1_5
  #define SHCP_PIN P1_6
  #define NMR_PIN P1_7

#elif defined SIX_OWBUS_SLAVE_DEVICE

#elif defined MASTER_CONTROLLER_DEVICE

#elif defined MIXER_CONTROL_SLAVE_DEVICE

  // Define pins of the mixer controller
  #define CCW_PIN P1_5
  #define CW_PIN P1_6
  #define SPAREA_PIN P1_3
  #define USEBUFFER_PIN P1_4
  #define GPIO_PIN P3_7

#else
  #error "Device specific pinlayout definitions cannot be determined"
#endif

#endif /* PINLAYOUT_H_ */
