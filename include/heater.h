#pragma once

#include "config.h"

#include "pico/stdlib.h"

#include "hardware/gpio.h"


void init_heater();

void start_heater();

void stop_heater();

void set_duty_cycle(uint16_t duty);

void zerocross_detection(uint gpio, uint32_t events);

uint16_t get_heater_duty_cycle();

bool is_heater_enabled();