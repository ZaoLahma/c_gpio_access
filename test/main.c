#include "../inc/rpi_gpio_access.h"
#include <stdio.h>
#include <stdlib.h>

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
	EXPECT(0, exportPort(3));
	EXPECT(0, setPortDirection(3, OUT));
	EXPECT(0, unexportPort(3));
	
	return 0;
}
