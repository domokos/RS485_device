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
static __bit master_sm_state;
// After handling a host message does master need to listen for a
// response on the bus and relay it back to host
static bool master_wants_response_on_bus;
// The actual communication speed
static unsigned char comm_speed;

/*
 * Internal Utility functions
 */

// Set the direction of communication
static void
set_master_comm_state(unsigned char direction)
{
  COMM_DIRECTION_PIN = direction;
}

// Set master's communication direction when communicating on the bus
static void
set_master_bus_comm_direction(unsigned char direction)
{
  MASTER_BUS_COMM_DIRECTION_PIN = direction;
}

// Gets and returns message if this master is addressed
static bool
get_master_message()
{
  if (get_message())
    return get_host_address() == message_buffer.content[MASTER_ADDRESS];

  return FALSE;
}

static void
relay_message_to_bus(void)
{
  set_master_comm_state(MASTER_TALKS_TO_BUS);
  set_master_bus_comm_direction(MASTER_SENDS_ON_BUS);

  send_message(message_buffer.content[OPCODE]);
}

/*
 * Public module functions
 */

// Reset the state of the communication
void
reset_master_comm(void)
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

  // Reset the timer for watching unseen host communication
  reset_timeout(UNSEEN_COMM_TIMER);
}

// Initialize the master
void
init_master(unsigned char host_address, unsigned char _comm_speed)
{
  // Set the communication speed
  set_comm_speed(_comm_speed);
  comm_speed = _comm_speed;

  // Set the host address
  set_host_address(host_address);

  // Reset the communication channel
  reset_master_comm();
}

void
operate_master(void)
{

  while (TRUE)
    {
      if (master_sm_state == SM_MASTER_LISTENS_TO_HOST)
        {
          if (get_master_message())
            {
              master_wants_response_on_bus = TRUE;
              switch (message_buffer.content[OPCODE])
                {
              case SET_COMM_SPEED:
                relay_message_to_bus();
                // Set the communication speed and remember it
                set_comm_speed(message_buffer.content[PARAMETER_START]);
                comm_speed = message_buffer.content[PARAMETER_START];
                break;

              case PING_MASTER:
                // Respond with an echo
                send_message(MASTER_ECHO);
                // Do not start listening on bus
                master_wants_response_on_bus = FALSE;
                break;

              default:
                // Just relay the message and then change state
                relay_message_to_bus();
                break;
                }
              // If master relayed a message to the bus - and is already talking to bus then
              // start listening to the response - change state
              if (master_wants_response_on_bus)
                {
                  set_master_bus_comm_direction(MASTER_LISTENS_ON_BUS);
                  master_sm_state = SM_MASTER_LISTENS_ON_BUS;
                  reset_timeout(RESPONSE_TIMEOUT);
                }
            }
          else
            {
              // Make sure the direction pin is not stuck for some reason
              // will ruin an incoming message so let's hope it will not occur whithin
              // the timeout indicated
              if (timeout_occured(UNSEEN_COMM_TIMER, MASTER_UNSEEN_COMM_TIMEOUT))
                {
                  set_master_comm_state(MASTER_TALKS_TO_BUS);
                  set_master_comm_state(MASTER_TALKS_TO_HOST);
                  reset_timeout(UNSEEN_COMM_TIMER);
                }
            }
          // SM_MASTER_LISTENS_ON_BUS
        }
      else
        {
          if (get_master_message())
            {
              set_master_comm_state(MASTER_TALKS_TO_HOST);

              send_message(message_buffer.content[OPCODE]);

              master_sm_state = SM_MASTER_LISTENS_TO_HOST;
              reset_timeout(UNSEEN_COMM_TIMER);
            }
          else
            {
              // Handle messaging timeout and notify master of bus CRC error
              if (timeout_occured(RESPONSE_TIMEOUT,
                  comm_speeds[comm_speed].resp_timeout))
                {
                  set_master_comm_state(MASTER_TALKS_TO_HOST);

                  message_buffer.index = PARAMETER_START - 1;
                  send_message(TIMEOUT);

                  master_sm_state = SM_MASTER_LISTENS_TO_HOST;
                  reset_timeout(UNSEEN_COMM_TIMER);
                }
              if (get_comm_error() == COMM_CRC_ERROR)
                {
                  set_master_comm_state(MASTER_TALKS_TO_HOST);

                  message_buffer.index = PARAMETER_START - 1;
                  send_message(CRC_ERROR);

                  master_sm_state = SM_MASTER_LISTENS_TO_HOST;
                  reset_timeout(UNSEEN_COMM_TIMER);
                }
            }
        }
    }
}

