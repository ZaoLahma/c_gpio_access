#ifndef HEADER_RPI_GPIO_ACCESS
#define HEADER_RPI_GPIO_ACCESS

struct RpiGPIOAccess;

extern struct RpiGPIOAccess* init_rpi_gpio_access();

extern int export_port(struct RpiGPIOAccess* gpioAccess, unsigned int portNo);
extern int unexport_port(struct RpiGPIOAccess* gpioAccess, unsigned int portNo);

typedef enum GPIODirection
{
	IN,
	OUT
} GPIODirection;

extern int set_port_direction(struct RpiGPIOAccess* gpioAccess, unsigned int portNo, GPIODirection direction);

typedef enum GPIOVal
{
	HIGH = 1,
	LOW = 0
} GPIOVal;

extern int set_port_value(struct RpiGPIOAccess* gpioAccess, unsigned int portNo, GPIOVal val);

#endif
