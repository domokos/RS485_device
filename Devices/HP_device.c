/*
 * HP_device.c
 *
 *  Created on: Oct 31, 2022
 *      Author: doma
 */

#include "HP_device.h"

/*
 * Define registers of this device
 */
// This device has 10 registers
__code const unsigned char nr_of_registers = 12;

// Describe the registers of this device
__code const unsigned char register_identification[][REG_IDENTIFICATION_LEN] =
  {
      // Relay output - Contact Low Tariff
        { REG_TYPE_SW, REG_RW, 2, DONT_CARE, DONT_CARE },
      // Relay output - Contact Night Mode
        { REG_TYPE_SW, REG_RW, 2, DONT_CARE, DONT_CARE },
      // Relay output - Contact Heat/Cool Mode
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Relay output - Contact Dual Setpoint
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Relay output - Contact ON/OFF
        { REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },
      // Relay output - Contact DHW
	{ REG_TYPE_SW, REG_RW, 1, DONT_CARE, DONT_CARE },

      // HW temp wiper
	{ REG_TYPE_DATA, REG_RW, 2, DONT_CARE, DONT_CARE },
      // External temp - watertemp control - wiper
	{ REG_TYPE_DATA, REG_RW, 2, DONT_CARE, DONT_CARE },

      // 230V Input sensing - EHS
	{ REG_TYPE_SW, REG_RO, 1, DONT_CARE, DONT_CARE },
      // 230V Input sensing - Heat/Cool mode
	{ REG_TYPE_SW, REG_RO, 1, DONT_CARE, DONT_CARE },
      // 230V Input sensing - Backup Heater
	{ REG_TYPE_SW, REG_RO, 1, DONT_CARE, DONT_CARE },
      // 230V Input sensing - Alarm
	{ REG_TYPE_SW, REG_RO, 1, DONT_CARE, DONT_CARE }};

/* Input sensing event buffers */
unsigned char events_in_window[4];

static void
update_input_sensing_windows(void)
{
  unsigned char index, sensing_events=0, mask=0x01;
  if ( timeout_occured(INPUT_SENSING_TIMER, INPUT_SENSING_UPDATE_TIMEOUT) )
    {
      reset_timeout(INPUT_SENSING_TIMER);

      // Coollect states of each pin into a bitfield
      sensing_events =
	  (EHS_PIN << EHS_EVENTS) |
	  (MODE_PIN << MODE_EVENTS) |
	  (BACKUP_PIN << BACKUP_EVENTS) |
	  (ALARM_PIN << ALARM_EVENTS);
/*
Iterate through the sensed bits and increase or decrease events
Sensing is biased towards positive events as sensing HW senses AC and only the positive half-wave period
Sensing is negative logioc - a pin value of zero means an event has occured
*/
      for(index=0;index<=3;index++)
	{
	  if((sensing_events & mask) > 0)
	    {
	      if (events_in_window[index] > 0) events_in_window[index]--;
	    } else {
              if (events_in_window[index] < INPUT_SENSING_WINDOW_SIZE) events_in_window[index]+=3;
	    }
	  mask = mask << 1;
	}
    }
}


/* Device specific initailization */
static void
device_specific_init_phase1(void)
{
  // Set all pins to high to allow reading them
  EHS_PIN = MODE_PIN = BACKUP_PIN = ALARM_PIN = 1;

  // Turn off all outputs
  reset_extender();

  // Reset the MCP4161 rheostat
  reset_rheostats();

  // Enable the resistor network globally and
  // enable the terminals on the MCP4162 chip
  set_tcon(0x0b, WIPER_HEAT);
  set_tcon(0x0b, WIPER_HW);

  // Set safe values for wipers
  // HW 50k wiper target is max temp - 2k - value 2
  write_wiper(0x02,TRUE,WIPER_HW);
  write_wiper(0x02,FALSE,WIPER_HW);

  // HEAT 100k wiper target 20 C 16k  - value 40
  write_wiper(0x28,TRUE,WIPER_HEAT);
  write_wiper(0x28,FALSE,WIPER_HEAT);
}

/* Device specific initailization */
static void
device_specific_init_phase2(void)
{
  // Reset input sensing timer
  reset_timeout(INPUT_SENSING_TIMER);
  // Reset watchdog timer
  reset_timeout(EXTENDER_WATCHDOG_TIMER);
  write_extender(WD_EXTENDER_REGISTER, !get_extender_value(WD_EXTENDER_REGISTER));
}

static void
handle_watchdog(void)
{
  if(timeout_occured(EXTENDER_WATCHDOG_TIMER, HP_WD_TIMEOUT))
    {
      reset_timeout(EXTENDER_WATCHDOG_TIMER);
      // Emit watchdog pulse
      write_extender(WD_EXTENDER_REGISTER, 0);
      write_extender(WD_EXTENDER_REGISTER, 1);
    }
}

/* Main event loop of the device */
static void
operate_device(void)
{
  while (TRUE)
    {
      handle_watchdog();

      update_input_sensing_windows();

      // Messaging variables
      unsigned char response_opcode = RESPONSE_UNDEFINED, p;

      // Take care of messaging
      if (get_device_message() && !process_generic_messages())
	{
	  // Set response opcode to undefined to filter response opcode programming issues
	  response_opcode = RESPONSE_UNDEFINED;

	  switch (message_buffer.content[OPCODE])
	    {
	  case SET_REGISTER:
	    // p holds register to write
	     p = message_buffer.content[PARAMETER_START];

	     // Preset response opcode to success
	     response_opcode = COMMAND_SUCCESS;
	     message_buffer.index = PARAMETER_START-1;

	     if (p>0 && p<=6)
	     {
 /*          Address 1:  Relay Low tariff
 *           Address 2:  Relay Night mode
 *           Address 3:  Relay Heat/Cool mode
 *           Address 4:  Relay Dual setpoint
 *           Address 5:  Relay ON/OFF
 *           Address 6:  Relay DHW
 */
		 write_extender(p, message_buffer.content[PARAMETER_START + 1] > 0);
	     } else if(p>=7 && p<=8) {
/*
*           Temp wipers - expected data format:
*           Byte 1 & 2 - 9 bits of data holding the desired wiper setting
*           Byte 3 - bool flag - is volatile
*           Address 7: HW Wiper
*           Address 8: Heating Wiper / External temp
*/
		 if (!write_wiper(
		     (message_buffer.content[PARAMETER_START+1] << 8) | message_buffer.content[PARAMETER_START+2] ,
		     message_buffer.content[PARAMETER_START + 3]>0,
		     p == 8 ? WIPER_HP_HW : WIPER_HP_HEAT))
		   response_opcode = COMMAND_FAIL;

	     } else {
/*
* 	     Any other addresses fail
* 	     Remaining registers are read only
*/
		   response_opcode = COMMAND_FAIL;
	     }
	     break;
	   case READ_REGISTER:
	    // p holds register to read
	     p = message_buffer.content[PARAMETER_START];

	     // Preset response opcode to success
	     response_opcode = COMMAND_SUCCESS;
	     message_buffer.index = PARAMETER_START-1;

	     if (p>0 && p<=6)
	     {
/*
* 	    Address 1:  Relay Low tariff
*           Address 2:  Relay Night mode
*           Address 3:  Relay Heat/Cool mode
*           Address 4:  Relay Dual setpoint
*           Address 5:  Relay ON/OFF
*           Address 6:  Relay DHW
*/
		 message_buffer.content[PARAMETER_START] = get_extender_value(p) > 0 ? 1 : 0;
		 message_buffer.index = PARAMETER_START;
	     } else if(p>=7 && p<=8) {
/*
*           Temp wipers - expected data format:
*           Byte 1 & 2 - 9 bits of data holding the desired wiper setting
*           Byte 3 - bool flag - is volatile
*           Address 7: HW Wiper
*           Address 8: Heating Wiper / External temp
*/
	         if(read_wiper((unsigned int*)(message_buffer.content+PARAMETER_START),
			       message_buffer.content[PARAMETER_START+1]>0,
			       p == 7 ? WIPER_HP_HW : WIPER_HP_HEAT))
	           {
	             message_buffer.index = PARAMETER_START+1;
	           } else {
	  	     response_opcode = COMMAND_FAIL;
	  	     message_buffer.index = PARAMETER_START-1;
	           }
	     } else if(p>=9 && p<=12) {
/*
* 	    Address 9:  Input EHS
*           Address 10: Input Heat/Cool mode
*           Address 11: Input Backup heater
*           Address 12: Input Alarm
*/
		 message_buffer.content[PARAMETER_START] =
		     events_in_window[p-9] > EVENTS_IN_WINDOW_THRESHOLD ? 1 : 0;
		 message_buffer.index = PARAMETER_START;
	     } else {
/*
* 	     Any other addresses fail
*/
	       response_opcode = COMMAND_FAIL;
	       message_buffer.index = PARAMETER_START-1;
	     }
	     break;
	    // Any other opcode fails
	   default:
	     response_opcode = COMMAND_FAIL;
	     message_buffer.index = PARAMETER_START-1;
	     break;
	    }
	  send_response(response_opcode);
	}
      }
}

#if 0
static void
test(void)
{
  unsigned int counter=0;
  unsigned char hi,lo;
  while(TRUE)
    {
    handle_watchdog();
    delay_msec(10);
    counter++;
    if (counter == 150)
      {
	hi = 0x00;
	lo = 0x00;
	write_wiper(hi << 8 | lo, TRUE, WIPER_HW);
	write_wiper(hi << 8 | lo, TRUE, WIPER_HEAT);
	for (hi=1;hi<=6;hi++)
	  write_extender(hi, 1);
      }
    if (counter == 300)
      {
	hi = 0x00;
	lo = 0x0a;
	write_wiper(hi << 8 | lo, TRUE, WIPER_HW);
	write_wiper(hi << 8 | lo, TRUE, WIPER_HEAT);
	for (hi=1;hi<=6;hi++)
	  write_extender(hi, 0);
      }
    if (counter == 450)
      {
	hi = 0x00;
	lo = 0xa0;
	write_wiper(hi << 8 | lo, TRUE, WIPER_HW);
	write_wiper(hi << 8 | lo, TRUE, WIPER_HEAT);
	for (hi=1;hi<=6;hi++)
	  write_extender(hi, 1);
      }
    if (counter == 600)
      {
	hi = 0x00;
	lo = 0xaa;
	write_wiper(hi << 8 | lo, TRUE, WIPER_HW);
	write_wiper(hi << 8 | lo, TRUE, WIPER_HEAT);
        counter = 0;
	for (hi=1;hi<=6;hi++)
	  write_extender(hi, 0);
      }
    }
}
#endif

/* Device entry point */
void
main(void)
{

  device_specific_init_phase1();

  // Enable interrupts and initialize timer
  EA = 1;
  init_timer();

  // Initialize device communication
  init_device_comm(HOST_ID, COMM_SPEED_9600_H);

  device_specific_init_phase2();

//test();

  // Start the main event loop
  operate_device();
}
