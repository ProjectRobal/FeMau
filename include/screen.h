#pragma once

#include <stdint.h>

#include "oled.h"

void init_screen();

void update_screen(int32_t temperature,int32_t target_temperature);