/*
 * Furnace_temp_device.h
 *
 *  Created on: Feb 19, 2013
 *      Author: dmolnar
 */

#ifndef FURNACE_TEMP_DEVICE_H
#define FURNACE_TEMP_DEVICE_H

#include "Slave_comm.h"

// The masks used to manipulate temp timeout values
#define ONEWIRE_TEMP_FAIL 0xaf0f

#define TEMP_RESOLUTION_12BIT 0x7F


bool set_temp_resolution(unsigned char register_id, unsigned char resolution);
void scale_DS18B20_result(unsigned char register_id);
void send_onewire_rom_commands(unsigned char register_id);
void read_DS18xxx(unsigned char register_id);
bool issue_convert_on_bus(unsigned char register_id);
void operate_onewire_temp_measurement(void);


void reset_extender_switches(void);
void write_extender_switches(void);
__bit get_extender_switch_buffer(unsigned char reg_nr);
void set_extender_switch_buffer(unsigned char reg_nr, __bit value);
void set_new_pwm_values(void);
void activate_pwm_state(unsigned char next_pwm_state);
unsigned char evaluate_state_change_rule1(void);
unsigned char evaluate_state_change_rule2(void);
unsigned char evaluate_state_change_rule3(void);
void operate_PWM(void);


void operate_device(void);
void device_specific_init(void);
void main(void);

#endif /* FURNACE_TEMP_DEVICE_H */
