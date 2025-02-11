#include "config.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "adc_sensors.h"
#include "heater.h"
#include "buttons.h"

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


int main() {

    stdio_init_all();
    printf("Hello, world!\n");

    printf("ADC init \n");
    // init ADC sensors
    init_adc_sensors();

    printf("Heater init \n");
    init_heater();

    printf("Buttons init \n");
    // init_buttons();

    // start_heater();

    bool enabled = false;

    sleep_ms(1000);


    while(1)
    {
        
        int32_t temperature = read_temperature();
        // printf("TC:%d\n",temperature); 

        bool check_enable = check_enabled();

        if( enabled != check_enable )
        {
            if(check_enable)
            {
                // start_heater();
                printf("HS:1\n");
            }
            else
            {
                // stop_heater();
                printf("HS:0\n");
            }

            enabled = check_enable;
        }

        // main loop automatic or manual

        if( check_mode() )
        {
            // automatic mode

            int32_t target = read_target_temperature();

            // printf("TA:%d\n",target);

            // PID goes brrrrrr
        }
        else
        {
            // manual mode

            uint16_t power = read_power();

            // printf("P:%d\n",power);

            set_duty_cycle(power);

            // regulated by human user
        }
    
        
        sleep_ms(MAIN_LOOP_REFRESH_MS);
    }

    
    return 0;
}