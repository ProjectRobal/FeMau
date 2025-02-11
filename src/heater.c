#include "heater.h"

#include <stdio.h>


// from 0 to 4095
static volatile uint16_t duty_length;

static inline void reset_pwm()
{
    gpio_put(HEATER_OUTPUT,false);
}

static int64_t reset_pwm_callback(alarm_id_t id, void *user_data) {

    reset_pwm();

    return 0;
}

// PWM will be driven by interrupts and timers

static void zerocross_detection(uint gpio,uint32_t events)
{
    printf("Cross detected\n");
    // start PWM timer
    if( duty_length > 0 )
    {
        gpio_put(HEATER_OUTPUT,true);

        printf("Cycle started\n");

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

    gpio_set_irq_enabled_with_callback(ZEROCROSS_DETECTOR_PIN, GPIO_IRQ_EDGE_RISE, true, &zerocross_detection);

}

void start_heater()
{

    reset_pwm();

    gpio_set_irq_enabled(ZEROCROSS_DETECTOR_PIN,GPIO_IRQ_EDGE_RISE,true);

}

void stop_heater()
{
    
    gpio_set_irq_enabled(ZEROCROSS_DETECTOR_PIN,GPIO_IRQ_EDGE_RISE,false);

    reset_pwm();

}

void set_duty_cycle(uint16_t duty)
{
    if( duty > 4095 )
    {
        duty = 4095;
    }

    duty_length = duty;
}

uint16_t get_duty_cycle()
{
    return duty_length;
}