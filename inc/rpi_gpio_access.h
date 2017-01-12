#ifndef HEADER_RPI_GPIO_ACCESS
#define HEADER_RPI_GPIO_ACCESS

struct RpiGPIOAccess;

extern struct RpiGPIOAccess* init_rpi_gpio_access();

extern int exportPort(struct RpiGPIOAccess* gpioAccess, unsigned int portNo);
extern int unexportPort(struct RpiGPIOAccess* gpioAccess, unsigned int portNo);

typedef enum GPIODirection
{
	IN,
	OUT
} GPIODirection;

extern int setPortDirection(struct RpiGPIOAccess* gpioAccess, unsigned int portNo, GPIODirection direction);

typedef enum GPIOVal
{
	HIGH = 1,
	LOW = 0
} GPIOVal;

extern int setPortValue(struct RpiGPIOAccess* gpioAccess, unsigned int portNo, GPIOVal val);

#endif
