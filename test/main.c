#include "../inc/rpi_gpio_access.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EXPECT(this, that) \
if(this != that) \
{\
	printf("\n---- TEST FAILED ----\n");\
	printf("File: %s, line: %d\n", __FILE__, __LINE__);\
	printf(#this " (0x%x) != " #that " (0x%x)\n", \
		  (unsigned int)this,\
		  (unsigned int)that);\
	abort();\
}\
else\
{\
	printf(#this " == " #that " validated OK\n");\
}

int main(void)
{
	struct RpiGPIOAccess* gpioAccess = init_rpi_gpio_access();
	
	EXPECT(0, export_port(gpioAccess, 7));
	EXPECT(0, set_port_direction(gpioAccess, 7, OUT));
	EXPECT(0, set_port_value(gpioAccess, 7, HIGH));
	usleep(1000 * 1000);
	EXPECT(0, set_port_value(gpioAccess, 7, LOW));
	usleep(1000 * 1000);
	EXPECT(0, set_port_value(gpioAccess, 7, HIGH));
	usleep(1000 * 1000);
	EXPECT(0, set_port_value(gpioAccess, 7, LOW));
	usleep(1000 * 1000);
	EXPECT(0, set_port_value(gpioAccess, 7, HIGH));
	usleep(1000 * 1000);
	EXPECT(0, set_port_value(gpioAccess, 7, LOW));
	usleep(1000 * 1000);
	EXPECT(0, unexport_port(gpioAccess, 7));
	free(gpioAccess);
	
	return 0;
}
