#include "../inc/rpi_gpio_access.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NO_OF_GPIO 16
struct GPIOPin
{
	int fd;
	GPIODirection direction;
};
struct RpiGPIOAccess
{
	struct GPIOPin pins[NO_OF_GPIO];
};

struct RpiGPIOAccess* init_rpi_gpio_access()
{
	struct RpiGPIOAccess* retVal = (struct RpiGPIOAccess*)malloc(sizeof(struct RpiGPIOAccess));
	
	return retVal;
}

static int board_to_GPIO_pin(unsigned int portNo)
{
	switch (portNo) {
	case 3:
		return 0;
	case 5:
		return 1;
	case 7:
		return 2;
	case 8:
		return 3;
	case 10:
		return 4;
	case 11:
		return 5;
	case 12:
		return 6;
	case 13:
		return 7;
	case 16:
		return 8;
	case 18:
		return 9;
	case 19:
		return 10;
	case 21:
		return 11;
	case 22:
		return 12;
	case 23:
		return 13;
	case 24:
		return 14;
	case 26:
		return 15;
	default:
		printf("No such port: %d\n", portNo);
		return -1;
	}	
}

static int board_to_bcm(unsigned int portNo)
{
	switch (portNo) {
	case 3:
		return 2;
	case 5:
		return 3;
	case 7:
		return 4;
	case 8:
		return 14;
	case 10:
		return 15;
	case 11:
		return 17;
	case 12:
		return 18;
	case 13:
		return 27;
	case 16:
		return 23;
	case 18:
		return 24;
	case 19:
		return 10;
	case 21:
		return 9;
	case 22:
		return 25;
	case 23:
		return 11;
	case 24:
		return 8;
	case 26:
		return 7;
	default:
		printf("No such port: %d\n", portNo);
		return -1;
	}
}

int export_port(struct RpiGPIOAccess* gpioAccess, unsigned int portNo)
{
	int bcmPortNo = board_to_bcm(portNo);
	
	if(-1 != bcmPortNo)
	{	
		int exportFd;
		exportFd = open("/sys/class/gpio/export", O_WRONLY);
		
		if(-1 != exportFd)
		{
			const unsigned int BUF_MAX = 3;
			char buf[BUF_MAX];
			memset(buf, 0, BUF_MAX);
			int bytesToWrite = snprintf(buf, BUF_MAX, "%d", bcmPortNo);
			write(exportFd, buf, bytesToWrite);
			close(exportFd);
			usleep(1000 * 500);
			
			const unsigned int PORT_FD_PATH_MAX = 29;
			char gpioPath[PORT_FD_PATH_MAX];
			memset(gpioPath, 0, PORT_FD_PATH_MAX);
			snprintf(gpioPath, PORT_FD_PATH_MAX, "/sys/class/gpio/gpio%d/value", bcmPortNo);
			gpioAccess->pins[board_to_GPIO_pin(portNo)].fd = open(gpioPath, O_RDWR | O_SYNC);
			gpioAccess->pins[board_to_GPIO_pin(portNo)].direction = IN;
			
			return 0;
		}
		return -2;
	}
	
	return -1;
}

int unexport_port(struct RpiGPIOAccess* gpioAccess, unsigned int portNo)
{
	int bcmPortNo = board_to_bcm(portNo);
	
	if(-1 != bcmPortNo)
	{	
		int unexportFd;
		unexportFd = open("/sys/class/gpio/unexport", O_WRONLY);
		
		if(-1 != unexportFd)
		{	
			close(gpioAccess->pins[board_to_GPIO_pin(portNo)].fd);
			gpioAccess->pins[board_to_GPIO_pin(portNo)].fd = -1;
			gpioAccess->pins[board_to_GPIO_pin(portNo)].direction = IN;
			
			const unsigned int BUF_MAX = 3;
			char buf[BUF_MAX];
			memset(buf, 0, BUF_MAX);
			int bytesToWrite = snprintf(buf, BUF_MAX, "%d", bcmPortNo);
			write(unexportFd, buf, bytesToWrite);
			close(unexportFd);
			return 0;
		}
		return -2;
	}
	
	return -1;	
}

int set_port_direction(struct RpiGPIOAccess* gpioAccess, unsigned int portNo, GPIODirection direction)
{
	int bcmPortNo = board_to_bcm(portNo);
	
	if(-1 != bcmPortNo)
	{
		int directionFd;
		const unsigned int DIR_PATH_MAX_LENGTH = 35;
		char directionPath[DIR_PATH_MAX_LENGTH];
		memset(directionPath, 0, DIR_PATH_MAX_LENGTH);
		
		snprintf(directionPath, DIR_PATH_MAX_LENGTH, "/sys/class/gpio/gpio%d/direction", bcmPortNo);
		
		directionFd = open(directionPath, O_WRONLY);
		
		if(-1 != directionFd)
		{
			const unsigned int MAX_DIR_LENGTH = 4;
			char directionString[MAX_DIR_LENGTH];
			memset(directionString, 0, MAX_DIR_LENGTH);
			int bytesToWrite;
			
			switch(direction)
			{
				case IN:
					bytesToWrite = snprintf(directionString, MAX_DIR_LENGTH, "%s", "in");
				break;
				case OUT:
					bytesToWrite = snprintf(directionString, MAX_DIR_LENGTH, "%s", "out");
				break;
				default:
				break;
			}
			write(directionFd, directionString, bytesToWrite);
			close(directionFd);
			
			gpioAccess->pins[board_to_GPIO_pin(portNo)].direction = direction;
			
			return 0;	
		}
		
		printf("Failed to open %s for writing\n", directionPath);
		return -2;
	}
	
	return -1;
}

int set_port_value(struct RpiGPIOAccess* gpioAccess, unsigned int portNo, GPIOVal val)
{
	if(-1 != gpioAccess->pins[board_to_GPIO_pin(portNo)].fd &&
	   IN != gpioAccess->pins[board_to_GPIO_pin(portNo)].direction)
	{
		const unsigned int BUF_LENGTH = 2;
		char valStr[BUF_LENGTH];
		memset(valStr, 0, BUF_LENGTH);
		int bytesToWrite;
		bytesToWrite = snprintf(valStr, BUF_LENGTH, "%d", val);
		write(gpioAccess->pins[board_to_GPIO_pin(portNo)].fd, valStr, bytesToWrite);
		
		return 0;
	}
	return -1;
}
