#ifndef HEADER_RPI_GPIO_ACCESS
#define HEADER_RPI_GPIO_ACCESS

struct RpiGPIOAccess;

extern int exportPort(unsigned int portNo);
extern int unexportPort(unsigned int portNo);

typedef enum GPIODirection
{
	IN,
	OUT
} GPIODirection;

extern int setPortDirection(unsigned int portNo, GPIODirection direction);

typedef enum GPIOVal
{
	HIGH,
	LOW
} GPIOVal;

extern int setPortValue(unsigned int portNo, GPIOVal val);

#endif
