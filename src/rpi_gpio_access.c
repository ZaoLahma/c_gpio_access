#include "../inc/rpi_gpio_access.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct RpiGPIOAccess
{
	
};

static int boardToBcm(unsigned int portNo)
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

int exportPort(unsigned int portNo)
{
	int bcmPortNo = boardToBcm(portNo);
	
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
			return 0;
		}
		return -2;
	}
	
	return -1;
}

int unexportPort(unsigned int portNo)
{
	int bcmPortNo = boardToBcm(portNo);
	
	if(-1 != bcmPortNo)
	{	
		int unexportFd;
		unexportFd = open("/sys/class/gpio/unexport", O_WRONLY);
		
		if(-1 != unexportFd)
		{
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

int setPortDirection(unsigned int portNo, GPIODirection direction)
{
	int bcmPortNo = boardToBcm(portNo);
	
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
			const unsigned int MAX_DIR_LENGTH = 3;
			char directionString[MAX_DIR_LENGTH];
			memset(directionString, 0, MAX_DIR_LENGTH);
			int bytesToWrite;
			
			switch(direction)
			{
				case IN:
					bytesToWrite = snprintf(directionString, MAX_DIR_LENGTH, "%s", "out");
				break;
				case OUT:
					bytesToWrite = snprintf(directionString, MAX_DIR_LENGTH, "%s", "in");
				break;
				default:
				break;
			}
			write(directionFd, directionString, bytesToWrite);
			close(directionFd);
			
			return 0;	
		}
		
		printf("Failed to open %s for writing\n", directionPath);
		return -2;
	}
	
	return -1;
}
