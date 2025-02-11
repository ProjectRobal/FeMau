#include "config.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "adc_sensors.h"

/*
    To do:

        - temperature measurment - done
        - potentiometer readings - done

        - zero cross detection
        - PWM control

        - PID regulator
        - automatic and manual control

        - some nice oled screen

*/

// PWM will be driven by interrupts and timers

int main() {

    stdio_init_all();
    printf("Hello, world!\n");

    // init ADC sensors
    init_adc_sensors();


    while(1)
    {
        
        int32_t temperature = read_temperature();

        // main loop automatic or manual

        int32_t target = read_target_temperature();

        printf("TC:%d\n",temperature);     
        printf("TA:%d\n",target);    
        
        sleep_ms(MAIN_LOOP_REFRESH_MS);
    }

    
    return 0;
}