/*
 * Master_comm.c
 *
 *  Created on: Feb 19, 2013
 *      Author: dmolnar
 */


#include "Master_comm.h"

/*
 * Global variables of the module
 */

// The state of the master state machine
__bit master_sm_state;
unsigned char comm_speed;

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
void init_master(unsigned char host_address, unsigned char _comm_speed)
{
  // Set the communication speed
  set_comm_speed(_comm_speed);
  comm_speed = _comm_speed;

  // Set the host address
  set_host_address(host_address);

  // Reset the communication channel
  reset_master_comm();
}


// Returns void* to the caller if no message is received
// returns a pointer to the message if a message is received
struct message_struct* get_master_message()
{
  struct message_struct* msg;

  if ((msg=get_message()) != NULL)
    {
    // If the master is the addressee of the message then check CRC
    if(get_host_address() == msg->content[MASTER_ADDRESS])
      {
       // If there is a CRC error then respond with a CRC error message and
       // do not return it to the caller
       if (get_comm_error() == COMM_CRC_ERROR)
         {
         msg -> index = PARAMETER_START-1;
         send_message(CRC_ERROR,msg->content[SEQ]);
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
  struct message_struct* message;

  while(TRUE)
    {
      if (master_sm_state == SM_MASTER_LISTENS_TO_HOST)
      {
        if ((message = get_master_message()) != NULL)
          {
            set_master_comm_state(MASTER_TALKS_TO_BUS);
            set_master_bus_comm_direction(MASTER_SENDS_ON_BUS);

            send_message(message->content[OPCODE],message->content[SEQ]);

            set_master_bus_comm_direction(MASTER_LISTENS_ON_BUS);
            master_sm_state = SM_MASTER_LISTENS_ON_BUS;

            reset_timeout(RESPONSE_TIMEOUT);

            // If command is SET_COMM_SPEED then set the communication speed
            if (message->content[OPCODE] == SET_COMM_SPEED)
              {
                set_comm_speed(message -> content[PARAMETER_START]);
                comm_speed = message -> content[PARAMETER_START];
              }
          }
        // SM_MASTER_LISTENS_ON_BUS
      } else {
        if ((message = get_master_message()) != NULL)
          {
            set_master_comm_state(MASTER_TALKS_TO_HOST);

            send_message(message->content[OPCODE],message->content[SEQ]);

            master_sm_state = SM_MASTER_LISTENS_TO_HOST;
          } else {
          // Handle messaging timeout and notify master of bus CRC error
            if ( timeout_occured(RESPONSE_TIMEOUT, comm_speeds[comm_speed].resp_timeout))
              {
                set_master_comm_state(MASTER_TALKS_TO_HOST);

                message -> index = PARAMETER_START-1;

                send_message(response_opcodes[TIMEOUT], message->content[SEQ]);

                master_sm_state = SM_MASTER_LISTENS_TO_HOST;
              }
            if (get_comm_error() == COMM_CRC_ERROR)
              {
                set_master_comm_state(MASTER_TALKS_TO_HOST);

                message -> index = PARAMETER_START-1;

                send_message(response_opcodes[CRC_ERROR], message->content[SEQ]);

                master_sm_state = SM_MASTER_LISTENS_TO_HOST;
              }
          }
      }
    }
}
