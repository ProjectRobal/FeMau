#pragma once

#include "class/cdc/cdc_device.h"

#define CONSOLE_BUFFER_SIZE 64

bool process_console_input(char c);

const char* console_get_value();

const char* console_get_header();