#include "config.h"

#include "pico/stdlib.h"

#include "hardware/gpio.h"

#include "buttons.h"

// button states
volatile static bool calibration;
volatile static bool automatic;
volatile static bool enabled;


void buttons_callback(uint gpio, uint32_t events)
{
    switch (gpio)
    {
    case START_BUTTON:
        
        enabled = !enabled;

        break;
    case CALIB_BUTTON:
        
        calibration = true;

        break;
    case AUTOMATIC_MANUAL_BUTTON:

        automatic = !automatic;

        break;
    
    default:
        break;
    }
}


void init_buttons()
{

    gpio_set_pulls(START_BUTTON,true,false);
    gpio_set_pulls(CALIB_BUTTON,true,false);
    gpio_set_pulls(AUTOMATIC_MANUAL_BUTTON,true,false);

    enabled = false;
    automatic = true;
    calibration = false;

    sleep_ms(10);


    gpio_set_irq_enabled_with_callback(START_BUTTON,GPIO_IRQ_EDGE_FALL,true,&buttons_callback);

    gpio_set_irq_enabled_with_callback(CALIB_BUTTON,GPIO_IRQ_EDGE_FALL,true,&buttons_callback);

    gpio_set_irq_enabled_with_callback(AUTOMATIC_MANUAL_BUTTON,GPIO_IRQ_EDGE_FALL,true,&buttons_callback);

}

bool check_enabled()
{
    return enabled;
}

bool calibration_needed()
{
    bool calib = calibration;

    calibration = false;

    return calib;
}

bool check_mode()
{
    return automatic;
}

