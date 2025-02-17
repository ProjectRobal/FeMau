#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "class/cdc/cdc_device.h"


#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

#include "adc_sensors.h"
#include "heater.h"
#include "buttons.h"
#include "pid.h"
#include "console.h"

#include "screen.h"

/*
    To do:

        - temperature measurment - done
        - potentiometer readings - done

        - zero cross detection - done
        - PWM control - done

        - buttons - done

        - PID regulator - done
        - automatic and manual control - done

        - console functions - done

        - some nice oled/lcd screen - done

        - calibration


*/

void process_console_messages();

inline void heater_enable_message();

inline void heater_disable_message();

inline void wrong_message_format();

void send_int_value(const char* header,int32_t value);

void send_pid_msg();

inline void send_current_duty_value();



volatile bool manual_override = false;
volatile uint16_t manual_power = 0;

volatile bool temperature_override = false;
volatile int32_t manual_temperature = 0;

volatile bool blink = false;


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

    puts("Screen init");
    init_screen();
    
    gpio_init(TEMPERATURE_INDICATOR);
    gpio_set_dir(TEMPERATURE_INDICATOR,true);


    bool enabled = false;

    bool last_mode = check_mode();

    uint8_t oled_counter = 5;

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
                    // etc
                }
            }

        }
        
        int32_t temperature = read_temperature();

        send_int_value("TC:",temperature);


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

        bool mode = check_mode();

        // mode changed event
        if( last_mode != mode )
        {
            // on switch to automatic disable manual override
            if( mode )
            {
                manual_override = false;
            }

            const char* header = "M:";
        
            fwrite(header,1,2,stdout);

            if( mode )
            {
                fwrite("1\n",1,2,stdout);
            }
            else
            {
                fwrite("0\n",1,2,stdout);
        }
            
            last_mode = mode;
        }

        // main loop automatic or manual

        int32_t target = 0;

        if( mode )
        {
            // automatic mode 

            if( temperature_override )
            {
                target = manual_temperature;
            }
            else
            {
                target = read_target_temperature();
            }

            send_int_value("TA:",target);

            int32_t error = target - temperature;

            if( abs(error) < 500 )
            {
                gpio_put(TEMPERATURE_INDICATOR,false);
            }
            else
            {
                gpio_put(TEMPERATURE_INDICATOR,true);
            }

            uint16_t power = pid_step(error);

            set_duty_cycle(power);

            send_current_duty_value();

            // PID goes brrrrrr
        }
        else
        {
            // manual mode

            uint16_t power = 0;

            if( manual_override )
            {
                power = manual_power;
            }
            else
            {
                power = read_power();
            }

            set_duty_cycle(power);

            send_current_duty_value();

            gpio_put(TEMPERATURE_INDICATOR,blink);

            blink = !blink;

            // regulated by human user
        }

        if( oled_counter == 0 )
        {
            update_screen(temperature,target);

            oled_counter = 5;
        }
    
        oled_counter--;

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
        send_pid_msg();   
    }
    // get current mode 1 is for automatic and 0 is for manual
    else if( strncmp(cmd,"MG",CONSOLE_BUFFER_SIZE) == 0 )
    {
        bool mode = check_mode();

        const char* header = "M:";
        
        fwrite(header,1,2,stdout);

        if( mode )
        {
            fwrite("1\n",1,2,stdout);
        }
        else
        {
            fwrite("0\n",1,2,stdout);
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

        if( power > 4095 )
        {
            wrong_message_format();
            return;
        }

        // set to manual mode
        set_mode(false);

        manual_override = true;
        
        manual_power = power;

    }
    // get current power
    else if( strncmp(cmd,"PG",CONSOLE_BUFFER_SIZE) == 0 )
    {
        send_current_duty_value();
    }
    // set current temperature
    else if( strncmp(cmd,"TS",CONSOLE_BUFFER_SIZE) == 0 )
    {
        manual_temperature = atoi(value);
        
        temperature_override = true;
    }
    // set pid values
    else if( strncmp(cmd,"PIDS",CONSOLE_BUFFER_SIZE) == 0 )
    {
        // P, I , D
        int32_t params[3] = {0};

        char buffer[255] = {0};

        uint16_t iterator = 0;

        uint16_t buffer_iter = 0;

        const uint32_t value_len = strlen(value); 

        for(uint8_t i=0;i<3;i++)
        {
            while( value[iterator] != ',' && value[iterator] != 0 && iterator <= value_len )
            {
                buffer[buffer_iter++] = value[iterator++];
            }

            if( iterator > value_len )
            {
                wrong_message_format();
                return;
            }

            params[i] = atoi(buffer);

            buffer_iter = 0;
            memset(buffer,0,255);

            // avoid ',' character
            iterator++;
        }

        if( params[0] == 0 )
        {
            wrong_message_format();
            return;
        }

        if( !setPIDParameters(params[0],params[1],params[2]) )
        {   
            // cannot save to flash
            puts("NFLS:");
        }
        else
        {
            // saved PID data to flash
            puts("FLS:");
        }

        send_pid_msg();

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

void send_int_value(const char* header,int32_t value)
{
    fwrite(header,1,strlen(header),stdout);

    char buffer[255] = {0};

    itoa(value,buffer,10);

    fwrite(buffer,1,strlen(buffer),stdout);

    fwrite("\n",1,1,stdout);
}

inline void send_current_duty_value()
{
    uint16_t power = get_heater_duty_cycle();

    const char* header = "PW:";
        
    send_int_value(header,power);

}

void send_pid_msg()
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