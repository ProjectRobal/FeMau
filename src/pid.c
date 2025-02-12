#include <string.h>

#include "pico/flash.h"
#include "hardware/flash.h"

#include "pid.h"

volatile static int32_t P;
volatile static int32_t I;
volatile static int32_t D;

volatile static int64_t integral;

volatile static int64_t last_error;

static inline void set_pid_default()
{
    P = 1000;
    I = 0;
    D = 0;
}

static void call_read_pid_from_flash(void* param)
{
    uint8_t* flash_ptr = (uint8_t*)( XIP_BASE + PID_FLASH_TARGET_OFFSET );

    int8_t sec_id = 0;

    sec_id = flash_ptr[0];

    if( sec_id != PID_FLASH_MAGIC_HEADER )
    {
        set_pid_default();
        return;
    }

    int32_t value = 0;

    memmove((uint8_t*)&value,flash_ptr+1,4);

    P = value;
    
    memmove((uint8_t*)&value,flash_ptr+5,4);

    I = value;

    memmove((uint8_t*)&value,flash_ptr+9,4);

    D = value;

}

static void read_pid_from_flash()
{
    int rc = flash_safe_execute(call_read_pid_from_flash, NULL, 5000);
    
    if( rc != PICO_OK )
    {
        // default PID value in case of error

        set_pid_default();

    }
}

static void call_erase_pid_flash(void* param)
{
    flash_range_erase(PID_FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
}

static void call_save_pid_to_flash(void* param)
{

    uint8_t buffer[256] = {0};

    buffer[0] = PID_FLASH_MAGIC_HEADER;

    int32_t value = P;

    memmove(buffer+1,(uint8_t*)&value,4);

    value = I;

    memmove(buffer+5,(uint8_t*)&value,4);

    value = D;

    memmove(buffer+9,(uint8_t*)&value,4);


    flash_range_program(PID_FLASH_TARGET_OFFSET,buffer,256);
}

static bool save_pid_to_flash()
{
    int rc = flash_safe_execute(call_erase_pid_flash,NULL,5000);

    if( rc != PICO_OK )
    {
        return false;
    }

    rc = flash_safe_execute(call_save_pid_to_flash, NULL, 5000);

    return rc != PICO_OK;
}




uint16_t pid_step(int32_t error)
{

    int64_t fixed_error = error;

    int64_t derror = error - last_error;

    integral += I*error;

    if( integral > 1000000 )
    {
        integral = 1000000;
    }

    if( integral < -1000000 )
    {
        integral = -1000000;
    }
    

    int16_t output = ( P*fixed_error + D*derror + integral ) / 1000;


    last_error = fixed_error;

    if( output < 0 )
    {
        output = 0;
    }

    if( output > 4095 )
    {
        output = 4095;
    }

    return output;

}

bool setPIDParameters(int32_t p,int32_t i,int32_t d)
{
    P = p;
    I = i;
    D = d;

    reset_pid();

    return save_pid_to_flash();
}

void reset_pid()
{
    integral = 0;
    last_error = 0;
}

void init_pid()
{
    // read values from flash
    read_pid_from_flash();

    reset_pid();
}

int32_t getP()
{
    return P;
}

int32_t getI()
{
    return I;
}

int32_t getD()
{
    return D;
}