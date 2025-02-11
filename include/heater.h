#pragma once

#include "config.h"

#include "pico/stdlib.h"

#include "hardware/gpio.h"


void init_heater();

void start_heater();

void stop_heater();