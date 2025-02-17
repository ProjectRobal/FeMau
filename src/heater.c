#include "heater.h"

#include <stdio.h>

#include "gpio_interrupt.h"


// from 0 to 4095
static volatile uint16_t duty_length;

static volatile bool heater_enable;



static inline void reset_pwm()
{
    gpio_put(HEATER_OUTPUT,false);
}

static int64_t reset_pwm_callback(alarm_id_t id, void *user_data) {

    reset_pwm();

    return 0;
}

// PWM will be driven by interrupts and timers

void zerocross_detection(uint gpio, uint32_t events)
{
    if( gpio != ZEROCROSS_DETECTOR_PIN )
    {
        return;
    }

    // printf("Cross detected\n");
    // start PWM timer
    if( duty_length > 0 )
    {
        gpio_put(HEATER_OUTPUT,true);

        // printf("Cycle started\n");

        add_alarm_in_us( (4395*duty_length)/1000 ,&reset_pwm_callback,NULL,false);
    }
    else
    {
        reset_pwm();
    }

}

void init_heater()
{

    duty_length = 0;

    gpio_init(HEATER_OUTPUT);
    gpio_set_dir(HEATER_OUTPUT,true);
    gpio_put(HEATER_OUTPUT,false);

    gpio_init(ZEROCROSS_DETECTOR_PIN);
    gpio_set_dir(ZEROCROSS_DETECTOR_PIN,false);

    heater_enable = true;

    gpio_set_irq_enabled_with_callback(ZEROCROSS_DETECTOR_PIN, GPIO_IRQ_EDGE_FALL, true,&gpio_interrupt_handler);

}

void start_heater()
{

    reset_pwm();

    gpio_set_irq_enabled(ZEROCROSS_DETECTOR_PIN,GPIO_IRQ_EDGE_FALL,true);

    heater_enable = true;

}

void stop_heater()
{
    
    gpio_set_irq_enabled(ZEROCROSS_DETECTOR_PIN,GPIO_IRQ_EDGE_FALL,false);

    reset_pwm();

    heater_enable = false;

}

void set_duty_cycle(uint16_t duty)
{
    if( duty > 4095 )
    {
        duty = 4095;
    }

    duty_length = duty;
}

uint16_t get_heater_duty_cycle()
{
    return duty_length;
}

bool is_heater_enabled()
{
    return heater_enable;
}