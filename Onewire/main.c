// Dallas 1-wire temperature sensor demo
// for use with the PJRC 8051 development board
// http://www.pjrc.com/tech/8051/board5/temperature.html


#define NICE_OUTPUT

// comment this to build without nice floating point at the end
#if 0



#include "Onewire.h"

#define CMD_READ_ROM		0x33
#define CMD_SKIP_ROM		0xCC
#define CMD_CONVERT_T		0x44
#define CMD_READ_SCRATCHPAD	0xBE
#define CMD_WRITE_SCRATCHPAD	0x4E

#ifdef NICE_OUTPUT
#define printf printf_fast_f
#else
#define printf printf_fast
#endif


void onewire_example(void)
{
	unsigned char i, buf[9], type, pinmask = 0x04;
	int dly, temperature;

//	printf("1-wire Temperature Test:\r\n");
	if (onewire_reset(pinmask)) {
//		printf("presence pulse ok\r\n");
		// It isn't really necessary to read the ROM if
		// you know which chip is connected.  A lot of
		// the code below can be removed if you only
		// use one type of sensor.
///		printf("ROM: ");
		onewire_write_byte(CMD_READ_ROM, pinmask);
		for (i=0; i<8; i++) {
			buf[i] = onewire_read_byte(pinmask);
//			printf("%x ", buf[i]);
		}
//		printf(" CRC=%s\r\n",
			onewire_crc_check(buf, 8);
		type = buf[0];
//		printf("Chip ID: ");
/*
		switch (type) {
		  case 0x10: printf("DS18S20\r\n"); break;
		  case 0x28: printf("DS18B20\r\n"); break;
		  case 0x22: printf("DS1822\r\n"); break;
		  default: printf("unknown\r\n"); break;
		}
*/
		// if it's a configurable resolution, set it for 12 bits
		if ((type == 0x28 || type == 0x22) && (buf[4] & 0x60) != 0x60) {
			onewire_write_byte(CMD_WRITE_SCRATCHPAD, pinmask);
			onewire_write_byte(0, pinmask);
			onewire_write_byte(0, pinmask);
			onewire_write_byte(0x7F, pinmask); // config
		}
//		printf("Begin temperature measurement\r\n");
		onewire_reset(pinmask);
		onewire_write_byte(CMD_SKIP_ROM, pinmask);
		onewire_write_byte(CMD_CONVERT_T, pinmask);
		//STRONG_PULLUP_PIN = 0;  // turn on strong pullup transistor
		for (dly=0; dly<1563; dly++) {
			delay_480us();
		}
		//STRONG_PULLUP_PIN = 1;  // turn off strong pullup
		onewire_reset(pinmask);
		// Temperature measurement is complete, now read it
		onewire_write_byte(CMD_SKIP_ROM, pinmask);
		onewire_write_byte(CMD_READ_SCRATCHPAD, pinmask);
//		printf("Data: ");
		for (i=0; i<9; i++) {
			buf[i] = onewire_read_byte(pinmask);
//			printf("%x ", buf[i]);
		}
//		printf(" CRC=%s\r\n",
//			onewire_crc_check(buf, 9);
		temperature = buf[0] | (buf[1] << 8);
		// if it's DS18S20, scale up to 12 bit resolution
		if (type == 0x10) {
			temperature *= 8;
			if (buf[7] == 0x10) {
				temperature &= 0xFFF0;
				temperature += 12;
				temperature -= buf[6];
			} else {
//				printf("Err: count remain not 0x10\r\n");
			}
		}
#ifdef NICE_OUTPUT
//		printf("Temperature: %.2f Celsius\r\n",
//			(float)temperature * 0.0625;
//		printf("Temperature: %.2f Fahrenheit\r\n",
//			(float)temperature * 0.1125 + 32.0;
#else
		// result is deg_C * 16
		printf("Temperature: %d / 16\r\n", temperature);
#endif
	} else {
//		printf("no 1-wire devices present\r\n");
	}


//	printf_fast("\r\n\r\n\r\npress any key to reboot");
//	pm2_cin();
//	pm2_restart();  /* ESC to quit */
}


#endif
