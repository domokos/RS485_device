/*
 * HP_device.h
 *
 *  Created on: Oct 31, 2022
 *      Author: doma
 */
#include "Generic_device.h"
#include "SingleExtender.h"

#include "../MCP416X/MCP4162.h"

#ifndef DEVICES_HP_DEVICE_H_
#define DEVICES_HP_DEVICE_H_

typedef __bit state;
#define ON 1
#define OFF 0

#define WIPER_HP_HW 0
#define WIPER_HP_HEAT 1

#define HP_WD_TIMEOUT 140
#define WD_EXTENDER_REGISTER 7

#define INPUT_SENSING_UPDATE_TIMEOUT 13
#define INPUT_SENSING_WINDOW_SIZE 150
#define EVENTS_IN_WINDOW_THRESHOLD 50
#define EHS_EVENTS 0
#define MODE_EVENTS 1
#define BACKUP_EVENTS 2
#define ALARM_EVENTS 3

static void update_input_sensing_windows(void);
static void device_specific_init_phase1(void);
static void device_specific_init_phase2(void);
static void handle_watchdog(void);
static void operate_device(void);



#endif /* DEVICES_HP_DEVICE_H_ */
