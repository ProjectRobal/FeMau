
#include "gpio_interrupt.h"

#include "buttons.h"
#include "heater.h"


void gpio_interrupt_handler(uint gpio, uint32_t events)
{
    buttons_callback(gpio,events);
    zerocross_detection(gpio, events);
}