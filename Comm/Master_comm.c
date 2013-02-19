/*
 * Master_comm.c
 *
 *  Created on: Feb 19, 2013
 *      Author: dmolnar
 */


#include "Base.h"
#include "Master_comm.h"

/*
 * Global variables of the module
 */

// The state of the master state machine
static unsigned char master_sm_state;


/*
 * Internal Utility functions
 */


// Set the direction of communication
static void set_master_comm_state(unsigned char direction)
{
  MASTER_COMM_DIRECTION_PIN = direction;
}

// Set master's communication direction when communicating on the bus
static void set_master_bus_comm_direction(unsigned char direction)
{
  MASTER_BUS_COMM_DIRECTION_PIN = direction;
}


/*
 * Public module functions
 */


// Reset the state of the communication
void reset_master_comm(void)
{

  // Listen to the host for commands
  set_master_comm_state(MASTER_TALKS_TO_HOST);

  // Nots used in initial state however this blocks any other
  // devices on the bus from acting as runaway masters
  set_master_bus_comm_direction(MASTER_SENDS_ON_BUS);

  // Reset the state machine
  master_sm_state = SM_MASTER_LISTENS_TO_HOST;

  // Reset the serial communication interface
  reset_comm();
}

// Initialize the master
void init_master(unsigned char host_address, unsigned char comm_speed)
{
  // Set the communication speed
  set_comm_speed(comm_speed);

  // Set the host address
  set_host_address(host_address);

  // Reset the communication channel
  reset_master_comm();
}


// Returns void* to the caller if no message is received
// returns a pointer to the message if a message is received
struct message_struct* get_master_message(unsigned int timeout_counter_limit)
{
  struct message_struct* msg;

  if ((msg=get_message(timeout_counter_limit)) != NULL)
    {
    // If the master is the addressee of the message then check CRC
    if(get_host_address() == msg->content[MASTER_ADDRESS])
      {
       // If there is a CRC error then respond with a CRC error message and
       // do not return it to the caller
       if (get_comm_error() == COMM_CRC_ERROR)
         {
         msg -> index = PARAMETER_START-1;
//         send_response(CRC_ERROR,msg->content[SEQ]);
         return NULL;
         } else {
         // CRC was OK return the message
         return msg;
       }
      }
    }
 return NULL;
}


void operate_master(void)
{
  while(TRUE)
    {
      switch (master_sm_state)
      {
      case SM_MASTER_LISTENS_TO_HOST:

        break;
      case SM_MASTER_SENDS_ON_BUS:
        break;
      case SM_MASTER_LISTENS_ON_BUS:
        break;
      case SM_MASTER_SENDS_TO_HOST:
        break;
      }
    }
}
