#pragma once

#include <stdint.h>

uint16_t pid_step(int32_t error);

void setPIDParameters(int32_t p,int32_t i,int32_t d);

void reset_pid();

void init_pid();

int32_t getP();

int32_t getI();

int32_t getD();