#include "config.h"

#include <math.h>

#include "adc_sensors.h"

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

    int32_t temperature = -363*raw + 1255170;

    return temperature;
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

