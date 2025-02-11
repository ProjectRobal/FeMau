#pragma once


void init_buttons();

bool check_enabled();

bool calibration_needed();

bool check_mode();

void buttons_callback(uint gpio, uint32_t events);