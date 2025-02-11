#pragma once

#include "hardware/gpio.h"
#include "hardware/adc.h"

void init_adc_sensors();

int32_t read_temperature();

uint16_t read_power();

uint32_t read_target_temperature();