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
 *  1 byte - data length
 *  1 byte - type specific value1
 *  1 byte - type specific value2
 */
extern __code const char register_identification[][REG_IDENTIFICATION_LEN];

// Register types
#define REG_TYPE_TEMP 0
#define REG_TYPE_SW 1
#define REG_TYPE_SW_EXTENDER 2

// Register read-wtite types
#define REG_RO 0
#define REG_RW 1

/*
 * Register type specific values
 */
// TEMP specific values:
// value1: 1 if measured temperature needs to be scled up to 12 bit
//         resolution, 0 otherwise
//
// value2: 1 if resolution is programmable 0 otherwise
//

#endif /* GENERIC_DEVICE_H_ */
