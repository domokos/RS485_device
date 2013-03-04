/*
 * Comm.c
 *
 *  Created on: Oct 20, 2012
 *      Author: Moln�r Domokos
 *
 *      This is a communication module for the AT89c4051 microcontroller
 *      over RS485
 */

#include "Base.h"
#include "Slave_comm.h"


// Set the direction of communication
static void set_device_comm_direction(unsigned char direction)
{
  COMM_DIRECTION_PIN = direction;
}

/*
 * Public interface functions
 */

// Reset the state of the communication channel
void reset_device_comm(void)
{
  // Listen on the bus for commands
  set_device_comm_direction(DEVICE_LISTENS);

  // Reset the serial communication interface
  reset_comm();
}

// Initialize the communication module
void init_device_comm(unsigned char host_address, unsigned char comm_speed)
{
  // Set the communication speed
  set_comm_speed(comm_speed);

  // Set the host address
  set_host_address(host_address);

  // Reset the communication channel
  reset_device_comm();
}

/* Function to send response to the master on the bus
 * the function expects message content to be prepared by the caller
 * including message SEQ number
*/
void send_response(unsigned char opcode)
{
  // Set bus direction to transmit
  set_device_comm_direction(DEVICE_SENDS);

  // Now send the message
  send_message(opcode);

  // Listen on the bus
  set_device_comm_direction(DEVICE_LISTENS);
}

// Returns void* to the caller if no message is received
// returns a pointer to the message if a message is received
bool get_device_message(void)
{
  if (get_message())
    {
    // If there is a CRC error then ignore the message
     if (get_comm_error() == COMM_CRC_ERROR) return FALSE;
       else return get_host_address() == message_buffer.content[SLAVE_ADDRESS];
    }
 return FALSE;
}
