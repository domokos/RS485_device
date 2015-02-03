/*
 * Generic_device.h
 *
 *  Created on: Mar 1, 2013
 *      Author: dmolnar
 */

#ifndef GENERIC_DEVICE_H_
#define GENERIC_DEVICE_H_

#include "Slave_comm.h"

// Returns true if message is processed and false otherwise
bool process_generic_messages();

#define REG_IDENTIFICATION_LEN 5
/*
 *  Must be defined with main
 *  Structure is:
 *  1 byte - register type
 *  1 byte - read/write type
 *  1 byte - length of data returned by the register
 *  1 byte - type specific value1
 *  1 byte - type specific value2
 */
extern __code const unsigned char register_identification[][REG_IDENTIFICATION_LEN];
extern __code const unsigned char nr_of_registers;

// Register types
#define REG_TYPE_TEMP 0 // Temperature store register
#define REG_TYPE_SW 1  // Switch register
#define REG_TYPE_INPUT 2 // Input register
#define REG_TYPE_DATA 3 // Data register

/*
 * Takes 3 parameters
 * 1st byte: On time in 10th seconds
 * 2nd byte: Off time in 10th seconds
 * 3rd byte: 1 - HI pin is PWM, while low pin is on
 *           0 - LO pin is PWM, while high pin is off
 */
#define REG_TYPE_PWM2 4  // 2 pin PWM switch control register

/*
 * This type of register takes a pulse length parameter which
 * corresponds to the pulse length required in 10th seconds
 */
#define REG_TYPE_PULSE 4 // Pulse output register

// Register read-wtite types
#define REG_RO 0
#define REG_RW 1
#define REG_WO 2

/*
 * Register type specific values
 */
// TEMP specific values:
// value1: 1 if measured temperature needs to be scled up to 12 bit
//         resolution, 0 otherwise
#define SCALE_POSITION 3
#define SCALE_TEMP 1
#define DONT_SCALE_TEMP 0

//
// value2: 1 if resolution is programmable 0 otherwise
//
#define RESOLUTION_POSITION 4
#define FIXED_RESOLUTION 0
#define PROG_RESOLUTION 1


// General register identification values
#define DONT_CARE 255

#endif /* GENERIC_DEVICE_H_ */
