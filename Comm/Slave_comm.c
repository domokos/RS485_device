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
  unsigned char i;
  if (get_message())
    {
    // If there is a CRC error then ignore the message
     if (get_comm_error() == COMM_CRC_ERROR) {
         P1_7=1;
         UART_putc(TRAIN_CHR);
         UART_putc(0xaa);
         for(i=0;i<=message_buffer.index+CRC2;i++)
         UART_putc(message_buffer.content[i]);
         UART_putc(0xaa);
         UART_putc(TRAIN_CHR);
         return FALSE;
     }
       else {P1_7 =0; return get_host_address() == message_buffer.content[SLAVE_ADDRESS]; }
    }
 return FALSE;
}
