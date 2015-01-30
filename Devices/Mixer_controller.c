/*
 * Mixer_controller.c
 *
 *  Created on: Jan 30, 2015
 *      Author: dmolnar
 */

#include "Generic_device.h"
#include "Mixer_controller.h"


// The id of this host on the bus
#define HOST_ID 12

/*
 * Define registers of this device
 */
// This device has 11 registers
__code const unsigned char nr_of_registers = 11;

#define NR_OF_TEMP_SENSORS 4
#define NR_OF_OW_BUSES 1

// Describe the registers of this device
__code const unsigned char register_identification[][REG_IDENTIFICATION_LEN] =
  {
      // HW temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, SCALE_TEMP, FIXED_RESOLUTION }, // DS18S20 - value1: scaling is needed(1), value2: fixed reesolution(0)
      // Basement temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, SCALE_TEMP, FIXED_RESOLUTION }, // DS18S20 - value1: scaling is needed(1), value2: fixed resolution(0)
      // Return temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)
      // Hidr Shift temp sensor
        { REG_TYPE_TEMP, REG_RW, 2, DONT_SCALE_TEMP, PROG_RESOLUTION }, // DS18B20 - value1: no scaling up needed(0), value2: programmable resolution(1)

      // Extender register outputs

      // Radiator pump - Extender bit 0 - Q0 - Contact 4
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Floor pump - Extender bit 1 - Q1 - Contact 5
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Hidraulic Shifter pump - Extender bit 2 - Q2 - Contact 6
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // HW pump - Extender bit 3 - Q3 - - Contact 7
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Basement floor valve - Extender bit 4 - Q4 - Contact 9
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Basement radiator valve - Extender bit 5 - Q5 - - Contact 8
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Buffertop valve - Extender bit 6 - Q6 - Contact 3
         { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Return valve - Extender bit 7 - Q7 - - Contact 2
         { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },

      // Heater relay P3_5
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },

      // Furnace temp wiper
        { REG_TYPE_DATA, REG_RW, 1, DONT_CARE, DONT_CARE },

      // HW temp wiper
        { REG_TYPE_DATA, REG_RW, 1, DONT_CARE, DONT_CARE } };


/*
 * Onewire specific declarations and defines
 */
// Map registers to onewire buses all registers are P3_3
__code const unsigned char register_pinmask_map[4] =
  {0x04, 0x04, 0x04, 0x04};

// Store 64 bit rom values of registers/devices
__code const unsigned char register_rom_map[][8] =
  {
  // If the first byte is zero, then there is only one device on bus
      // The HW sesor
        { 0x10, 0xc7, 0xa2, 0x23, 0x01, 0x08, 0x00, 0x74 },
      // The Basement sensor
        { 0x10, 0x47, 0xbf, 0x24, 0x01, 0x08, 0x00, 0x88 },
      // The return water temp sensor
        { 0x28, 0x91, 0x2d, 0x50, 0x01, 0x00, 0x00, 0xff },
      // The Hidr Shift water temp sensor
        { 0x28, 0x5a, 0xe6, 0x48, 0x01, 0x00, 0x00, 0xee }};

bool conv_complete;

// Bitfield to store register conversion initiation information
unsigned char register_conv_initiated;

// Buffer to store Temperatures and temp reading timeout
// temperatures are initialized @ 0C. At each unsuccesful reset or read attempt
// the value ONEWIRE_TEMP_FAIL is stored. This value is never the result of a succesful conversion
int temperatures[NR_OF_TEMP_SENSORS];

// Sensors are read in a circular manner. On e cycle completes in time equal to the conversion
// time. This variable holds the id of the sensor to be addressed next during the cycle.
unsigned char register_to_address;

void main(){
  while (TRUE)
  {



  }
}
