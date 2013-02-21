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
void send_response(unsigned char opcode, unsigned char seq)
{
  // Set bus direction to transmit
  set_device_comm_direction(DEVICE_SENDS);

  // Now send the message
  send_message(opcode, seq);

  // Listen on the bus
  set_device_comm_direction(DEVICE_LISTENS);
}

// Returns void* to the caller if no message is received
// returns a pointer to the message if a message is received
struct message_struct* get_device_message()
{
  struct message_struct* msg;

  if ((msg=get_message()) != NULL)
    {
    // If this slave is the addressee of the message then check CRC
    if(get_host_address() == msg->content[SLAVE_ADDRESS])
      {
       // If there is a CRC error then respond with a CRC error message and
       // do not return it to the caller
       if (get_comm_error() == COMM_CRC_ERROR)
         {
         msg -> index = PARAMETER_START-1;
         send_response(CRC_ERROR,msg->content[SEQ]);
         return NULL;
         } else {
         // CRC was OK return the message
         return msg;
       }
      }
    }
 return NULL;
}
