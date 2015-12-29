/*
 * eventcounter.c
 *
 *  Created on: May 31, 2015
 *      Author: dmolnar
 */

#include "Eventcounter.h"

// Global module variables
static volatile unsigned int  event_counter0, event_counter1, event_rate0, event_rate1;

/*
 * Module private functions
 */

ISR(EXT0,0)
  {
  event_counter0++;
  }

ISR(EXT1,0)
  {
  event_counter1++;
  }

static void update_event_counts()
{
    // Update event source 0
    EX0 = 0;
    event_rate0 = event_counter0;
    event_counter0 = 0;
    EX0 = 1;

    // Update event source 1
    EX1 = 0;
    event_rate1 = event_counter1;
    event_counter1 = 0;
    EX1 = 1;
}

/*
 * Module public functions
 */

// Must be called to initialize event counter
void init_event_counter()
{
  // Disable external interrupt
  EX0 = 0;
  EX1 = 0;

  // Reset all counters
  event_counter0 = event_counter1 = event_rate0 = event_rate1 = 0;

  // Enable evant sources
  EX0 = 1;
  EX1 = 1;

  // Begin event counting period
  reset_timeout(LEVEL_MEASUREMENT_TIMER);
}


// must be called periodically to maintain event rate
void operate_event_rate_measurement()
{
  // Update rate counters at the end of the if measurement period
  if(timeout_occured(LEVEL_MEASUREMENT_TIMER, ONE_SEC_TIMEOUT))
    update_event_counts();
}

unsigned int get_event_rate(event_source_type source)
{
  if(source == EXTERNAL0)
    {
      return event_rate0;
    // Source is EXTERNAL1
    }else{
      return event_rate1;
    }
}
