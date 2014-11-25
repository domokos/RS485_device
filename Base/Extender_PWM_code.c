/*
 * Extender_code.c
 *
 *  Created on: Nov 23, 2014
 *      Author: dmolnar
 */
#include "Generic_device.h"



void reset_extender_switches(void);
void write_extender_switches(void);
__bit get_extender_switch(unsigned char reg_nr);
void set_extender_switch_buffer(unsigned char reg_nr, __bit value);
void set_new_pwm_values(void);
void activate_pwm_state(unsigned char next_pwm_state);
unsigned char evaluate_state_change_rule1(void);
unsigned char evaluate_state_change_rule2(void);
unsigned char evaluate_state_change_rule3(void);
void operate_PWM(void);


#define NR_OF_SW_EXTENDERS 1

__code const unsigned char register_identification[][REG_IDENTIFICATION_LEN] =
    {
    // 2-pin PWM output on extender pins 0 and 1 - gas valves
      { REG_TYPE_PWM2, REG_RW, 3, DONT_CARE, DONT_CARE },

    }


/*
 * Extender switch specific declarations and defines.
 * For extender swich 74HC595, 8-bit parallell or serial out shift registers are used.
 * Extender switches need to be cascaded, meaning that the Q7S pin of registers need to be fed
 * into the DS of the next register, all other control lines should be connected.
 */

// This value holds the state of extender switches
unsigned char extender_sw_outputs[NR_OF_SW_EXTENDERS];

/*
 * PWM specific defines and variables
 */
// States and requested states
#define PWM_OFF 0
#define PWM_LO_OFF 1
#define PWM_MID 2
#define PWM_HI_ON 3
#define PWM_ON 4
#define PWM_REQ_HI 5
#define PWM_REQ_LO 6

// P1 pins of PWM
#define PWM_LO_PIN_ID 0
#define PWM_HI_PIN_ID 1

// Times for on/off when ramp from off to up or down
// 4 secs
#define PWM_RAMP_ON_TIME 40
#define PWM_RAMP_OFF_TIME 40

// Variables holding PWM times and state
unsigned char pwm_on_time, pwm_off_time, new_pwm_on_time, new_pwm_off_time,
    pwm_state, requested_PWM_state;

// Variables holding PWM modification related flags
bool is_pwm_low, new_is_pwm_low, load_new_pwm_values;


/*
 * EXTENDER Switches specific function section
 *
 */

// Reset the extender switches
void
reset_extender_switches(void)
{
  NOE_PIN = 1;
  DS_PIN = 0;
  SHCP_PIN = 0;
  NMR_PIN = 0;

  STCP_PIN = 0;
  STCP_PIN = 1;
  STCP_PIN = 0;

  NMR_PIN = 1;
  NOE_PIN = 0;
}

// Set the output to the values passed
void
write_extender_switches(void)
{
  unsigned char i, j, mask;

  j = NR_OF_SW_EXTENDERS;

  // Reset the shift registers
  NMR_PIN = 0;
  NMR_PIN = 1;

  // Loop through all the extender switch values
  while (j--)
    {
      i = extender_sw_outputs[j];
      mask = 0x80;
      while (mask)
        {
          DS_PIN = (i & mask) > 0;
          SHCP_PIN = 1;
          SHCP_PIN = 0;

          mask >>= 1;
        }
    }
  STCP_PIN = 1;
  STCP_PIN = 0;
}

// Return the value of the extender register referenced
// 0 is the first and 7 is the last in an 8 SW register
__bit
get_extender_switch(unsigned char reg_nr)
{
  unsigned char mask;

  mask = 0x01 << (reg_nr % 8);

  return (extender_sw_outputs[reg_nr / 8] & mask) > 0;
}

// Set the extender buffer value
// 0 is the first and 7 is the last in an 8 SW register
void
set_extender_switch_buffer(unsigned char reg_nr, __bit value)
{
  unsigned char mask;

  mask = 0x01 << (reg_nr % 8);

  if (value)
    extender_sw_outputs[reg_nr / 8] |= mask;
  else
    extender_sw_outputs[reg_nr / 8] &= ~mask;
}

// Set the new PWM vaues and return if state change logic needs to be reversed
void
set_new_pwm_values(void)
{

  bool set_values = TRUE;

  if (new_pwm_on_time == 0 && new_pwm_off_time == 1)
    {
      requested_PWM_state = PWM_OFF;
      set_values = FALSE;
    }
  else if (new_pwm_on_time == 1 && new_pwm_off_time == 0)
    {
      requested_PWM_state = PWM_ON;
      set_values = FALSE;
    }

  if (set_values)
    {
      // Load the new PWM values
      pwm_on_time = new_pwm_on_time;
      pwm_off_time = new_pwm_off_time;
      is_pwm_low = new_is_pwm_low;

      if (is_pwm_low)
        requested_PWM_state = PWM_REQ_LO;
      else
        requested_PWM_state = PWM_REQ_HI;
    }
  else
    {
      pwm_on_time = PWM_RAMP_ON_TIME;
      pwm_off_time = PWM_RAMP_OFF_TIME;
    }
  load_new_pwm_values = FALSE;
}

// Activate the PWM output values on the extender outputs and reset PWM timer
void
activate_pwm_state(unsigned char next_pwm_state)
{
  pwm_state = next_pwm_state;
  switch (pwm_state)
    {
  case PWM_OFF:
  case PWM_LO_OFF:
    set_extender_switch_buffer(PWM_LO_PIN_ID, 0);
    set_extender_switch_buffer(PWM_HI_PIN_ID, 0);
    break;
  case PWM_MID:
    set_extender_switch_buffer(PWM_LO_PIN_ID, 1);
    set_extender_switch_buffer(PWM_HI_PIN_ID, 0);
    break;
  case PWM_HI_ON:
  case PWM_ON:
    set_extender_switch_buffer(PWM_LO_PIN_ID, 1);
    set_extender_switch_buffer(PWM_HI_PIN_ID, 1);
    break;
    }
  write_extender_switches();
  reset_timeout(PWM1_TIMER);
}

unsigned char
evaluate_state_change_rule1(void)
{
  if (requested_PWM_state == PWM_OFF)
    return PWM_OFF;
  else
    return PWM_MID;
}

unsigned char
evaluate_state_change_rule2(void)
{
  if (requested_PWM_state == PWM_OFF)
    return PWM_OFF;
  else if (requested_PWM_state == PWM_REQ_LO)
    return PWM_LO_OFF;
  else if (requested_PWM_state == PWM_REQ_HI)
    return PWM_HI_ON;
  else
// If requested state is PWM_ON
    return PWM_ON;
}

unsigned char
evaluate_state_change_rule3(void)
{
  if (requested_PWM_state == PWM_ON)
    return PWM_ON;
  else
    return PWM_MID;
}

// Must be called periodically to take care of PWM outputs
void
operate_PWM(void)
{
  unsigned char next_state, wait_time;
  next_state = pwm_state;

  if (pwm_state == PWM_OFF || pwm_state == PWM_ON)
    {
      if (load_new_pwm_values)
        {
          set_new_pwm_values();
          if (pwm_state == PWM_OFF)
            next_state = evaluate_state_change_rule1();
          else
            next_state = evaluate_state_change_rule3();
        }
    }
  else
    {
      if (pwm_state == PWM_LO_OFF || (pwm_state == PWM_MID && !is_pwm_low))
        wait_time = pwm_off_time;
      else
        wait_time = pwm_on_time;

      if (timeout_occured(PWM1_TIMER, wait_time * 100))
        {
          if (!load_new_pwm_values)
            {
              if (requested_PWM_state == PWM_OFF)
                next_state = PWM_OFF;
              else if (requested_PWM_state == PWM_ON)
                next_state = PWM_ON;
              else if (pwm_state == PWM_MID && is_pwm_low)
                next_state = PWM_LO_OFF;
              else if (pwm_state == PWM_MID)
                next_state = PWM_HI_ON;
              else
                // if (pwm_state == PWM_HI_ON || pwm_state == PWM_LO_OFF)
                next_state = PWM_MID;
            }
          else
            {
              set_new_pwm_values();
              if (pwm_state == PWM_LO_OFF)
                next_state = evaluate_state_change_rule1();
              else if (pwm_state == PWM_MID)
                next_state = evaluate_state_change_rule2();
              else
                // if (pwm_state == PWM_HI_ON)
                next_state = evaluate_state_change_rule3();
            }
        }
    }

  if (next_state != pwm_state)
    activate_pwm_state(next_state);
}



void
device_specific_init(void)
{
  unsigned char i;

  // Reset the outputs of the extenders
  i = NR_OF_SW_EXTENDERS;
  while (i--)
    extender_sw_outputs[i] = 0;

  reset_extender_switches();

  // Reset PWM
  pwm_on_time = pwm_off_time = new_pwm_on_time = new_pwm_off_time = 0;
  is_pwm_low = new_is_pwm_low = TRUE;
  load_new_pwm_values = FALSE;
  pwm_state = requested_PWM_state = PWM_OFF;
}
