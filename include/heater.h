#pragma once

#include "config.h"

#include "pico/stdlib.h"

#include "hardware/gpio.h"


void init_heater();

void start_heater();

void stop_heater();

void set_duty_cycle(uint16_t duty);

uint16_t get_duty_cycle();