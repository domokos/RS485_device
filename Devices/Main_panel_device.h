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
#include "SingleExtender.h"

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
