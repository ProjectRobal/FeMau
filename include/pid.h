#pragma once

#include <stdint.h>

#define PID_FLASH_TARGET_OFFSET (256 * 1024)

#define PID_FLASH_MAGIC_HEADER 0xFC

uint16_t pid_step(int32_t error);

bool setPIDParameters(int32_t p,int32_t i,int32_t d);

void reset_pid();

void init_pid();

int32_t getP();

int32_t getI();

int32_t getD();