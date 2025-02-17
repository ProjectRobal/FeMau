#include "config.h"

#include <math.h>

#include "adc_sensors.h"

// should be less than 16
#define TEMP_BETA 4

#define FP_SHIFT 4

static uint32_t last_raw_value = 0; 

void init_adc_sensors()
{
    adc_init();

    adc_gpio_init(TEMPERATURE_SENSOR_PIN);

    adc_gpio_init(TEMPERATURE_SELECTION_PIN);

}

int32_t read_temperature()
{
    adc_select_input(TEMPERATURE_SENSOR_ID);

    int32_t raw = adc_read();

    // low pass filter
    raw <<= FP_SHIFT;
    last_raw_value = ( last_raw_value<<TEMP_BETA ) - last_raw_value;
    last_raw_value += raw;
    last_raw_value >>= TEMP_BETA; 

    int32_t smooth_value = last_raw_value >> FP_SHIFT;

    int32_t temperature = -363*smooth_value + 1255170;

    return temperature - 10000;
}


uint16_t read_power()
{
    adc_select_input(TEMPERATURE_SELECTION_ID);

    uint16_t selected = adc_read();

    return selected;
}

uint32_t read_target_temperature()
{
    uint32_t raw = read_power();

    uint32_t target = 74*raw - 1850;

    return target;
}

