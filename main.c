#include "config.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "class/cdc/cdc_device.h"


#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "adc_sensors.h"
#include "heater.h"
#include "buttons.h"
#include "pid.h"
#include "console.h"

/*
    To do:

        - temperature measurment - done
        - potentiometer readings - done

        - zero cross detection - done
        - PWM control - done

        - buttons - done

        - PID regulator - done
        - automatic and manual control - done

        - some nice oled screen

        - console functions

        - calibration

*/


int main() {

    stdio_init_all();

    sleep_ms(2000);

    puts("Hello, world!");

    puts("ADC init");
    // init ADC sensors
    init_adc_sensors();

    puts("Heater init");
    init_heater();

    puts("Buttons init");
    init_buttons();

    puts("PID init");
    init_pid();


    bool enabled = false;


    while(1)
    {

        if(tud_cdc_connected())
        {
            char c = tud_cdc_read_char();

            if( c > -1 )
            {
                if(process_console_input(c))
                {
                    // process message if anything was recived

                    puts(console_get_value());
                    puts(console_get_header());

                    // set PID values
                    // start/stop heater
                    // set Power
                }
            }

        }
        
        int32_t temperature = read_temperature();
        printf("TC:%d\n",temperature); 


        bool check_enable = check_enabled();

        if( enabled != check_enable )
        {
            if(check_enable)
            {
                start_heater();
                puts("HS:1");
            }
            else
            {
                stop_heater();
                puts("HS:0");
            }

            enabled = check_enable;
        }

        // main loop automatic or manual

        if( check_mode() )
        {
            // automatic mode

            int32_t target = read_target_temperature();

            printf("TA:%d\n",target);

            int32_t error = target - temperature;

            uint16_t power = pid_step(error);

            set_duty_cycle(power);

            // PID goes brrrrrr
        }
        else
        {
            // manual mode

            uint16_t power = read_power();

            printf("P:%d\n",power);

            set_duty_cycle(power);

            // regulated by human user
        }
    
        
        sleep_ms(MAIN_LOOP_REFRESH_MS);
    }

    
    return 0;
}