#include "config.h"

#include <stdlib.h>
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

        - console functions

        - some nice oled screen

        - calibration


*/

void process_console_messages();

inline void heater_enable_message();

inline void heater_disable_message();

inline void wrong_message_format();

volatile bool manual_mode = false;
volatile uint16_t manual_power = 0;

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

                    process_console_messages();

                    // set PID values
                    // get PID values
                    // start/stop heater
                    // set Power
                    // check status
                }
            }

        }
        
        int32_t temperature = read_temperature();
        // printf("TC:%d\n",temperature); 


        bool check_enable = check_enabled();

        if( enabled != check_enable )
        {
            if(check_enable)
            {
                start_heater();
                heater_enable_message();
            }
            else
            {
                stop_heater();
                heater_disable_message();
            }

            enabled = check_enable;
        }

        // main loop automatic or manual

        if( check_mode() )
        {
            // automatic mode

            int32_t target = read_target_temperature();

            // printf("TA:%d\n",target);

            int32_t error = target - temperature;

            uint16_t power = pid_step(error);

            set_duty_cycle(power);

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


void process_console_messages()
{

    const char* cmd = console_get_header();
    const char* value = console_get_value();

    // ping message
    if( strncmp(cmd,"HI",CONSOLE_BUFFER_SIZE) == 0 )
    {
        puts("HELLO:");
    }
    // get PID values
    else if( strncmp(cmd,"PIDG",CONSOLE_BUFFER_SIZE) == 0 )
    {
        const char* header = "PID:";
        
        fwrite(header,1,4,stdout);

        int32_t p = getP();
        int32_t i = getI();
        int32_t d = getD();

        char buff[255] = {0};

        itoa(p,buff,10);

        fwrite(buff,1,strlen(buff),stdout);

        memset(buff,0,255);
        
        fwrite(",",1,1,stdout);


        itoa(i,buff,10);

        fwrite(buff,1,strlen(buff),stdout);

        memset(buff,0,255);

        fwrite(",",1,1,stdout);


        itoa(d,buff,10);

        fwrite(buff,1,strlen(buff),stdout);

        fwrite("\n",1,1,stdout);
    }
    // get current mode 1 is for automatic and 0 is for manual
    else if( strncmp(cmd,"MG",CONSOLE_BUFFER_SIZE) == 0 )
    {
        bool mode = check_mode();

        const char* header = "M:";
        
        fwrite(header,1,4,stdout);

        if( mode )
        {
            puts("1");
        }
        else
        {
            puts("0");
        }

    }
    // start/stop heater
    else if( strncmp(cmd,"HS",CONSOLE_BUFFER_SIZE) == 0 )
    {
        if( strncmp(value,"1",CONSOLE_BUFFER_SIZE) == 0 )
        {
            start_heater();
            heater_enable_message();
        }
        else if( strncmp(value,"0",CONSOLE_BUFFER_SIZE) == 0 )
        {
            stop_heater();
            heater_disable_message();
        }
        else
        {
            wrong_message_format();
        }
        
    }
    // get heater status
    else if( strncmp(cmd,"HSS",CONSOLE_BUFFER_SIZE) == 0 )
    {
        if( is_heater_enabled() )
        {
            heater_enable_message();
        }
        else
        {
            heater_disable_message();
        }
    }
    // set power
    else if( strncmp(cmd,"PS",CONSOLE_BUFFER_SIZE) == 0 )
    {
        uint16_t power = atoi(value);

        
    }

}


inline void heater_enable_message()
{
    puts("HS:1");
}

inline void heater_disable_message()
{
    puts("HS:0");
}

inline void wrong_message_format()
{
    puts("NOK:");
}