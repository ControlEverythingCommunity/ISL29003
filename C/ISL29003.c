// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// ISL29003
// This code is designed to work with the ISL29003_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Light?sku=ISL29003_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, ISL29003 I2C address is 0x44(68)
	ioctl(file, I2C_SLAVE, 0x44);

	// Select command register(0x00)
	// Normal operation, 16-bit resolution(0x80)
	char config[2] = {0};
	config[0] = 0x00;
	config[1] = 0x80;
	write(file, config, 2);
	// Select control register(0x01)
	// Gain = 64000 lux(0x0C)
	config[0] = 0x01;
	config[1] = 0x0C;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data from register (0x04)
	// luminance lsb, luminance msb
	char reg[1] = {0x04};
	write(file, reg, 1);
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		// Convert the data
		int luminance = (data[1] * 256 + data[0]);

		// Output data to screen
		printf("Ambient Light Luminance : %d lux \n", luminance);
	}
}
