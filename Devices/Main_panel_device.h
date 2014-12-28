/*
 * Furnace_temp_device.h
 *
 *  Created on: Feb 19, 2013
 *      Author: dmolnar
 */

#ifndef FURNACE_TEMP_DEVICE_H
#define FURNACE_TEMP_DEVICE_H

#include "Slave_comm.h"
#include "MCP4161.h"
#include "Extender.h"

/*
 * Extender PIN mapping of the main panel device
 */

#define RADIATOR_PUMP 0x01 // Q0 - bit 0 - Contact 4
#define FLOOR_PUMP 0x02 // Q1 - bit 1 - Contact 5
#define HIDR_SHIFT_PUMP 0x04 // Q2 - bit 2 - Contact 6
#define HW_PUMP 0x08 // Q3 - bit 3 - Contact 7
#define BASEMENT_FLOOR_VALVE 0x10 // Q4 - bit 4 - Contact 9
#define BASEMENT_RADIATOR_VALVE 0x20 // Q5 - bit 5 - Contact 8
#define BUFFERTOP_VALVE 0x40 // Q6 - bit 6 - Contact 3
#define RETURN_VALVE 0x80 // Q7 - bit 7 - Contact 2

#define HEATER_RELAY_PIN P3_5 // Heater contact

// Onewire functions
bool set_temp_resolution_on_bus(unsigned char register_id, unsigned char resolution);
void scale_DS18S20_result(unsigned char register_id);
void send_onewire_rom_commands(unsigned char register_id);
void read_DS18xxx(unsigned char register_id);
bool issue_convert_on_bus(unsigned char register_id);
bool issue_convert_for_device(unsigned char register_id);
void operate_onewire_temp_measurement(void);


void operate_device(void);
void device_specific_init_phase1(void);
void device_specific_init_phase2(void);
void main(void);

#endif /* FURNACE_TEMP_DEVICE_H */
