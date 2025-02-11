#pragma once

#include "hardware/gpio.h"

void gpio_interrupt_handler(uint gpio, uint32_t events);