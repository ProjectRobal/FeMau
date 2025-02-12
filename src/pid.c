#include "pid.h"

volatile static int32_t P;
volatile static int32_t I;
volatile static int32_t D;

volatile static int64_t integral;

volatile static int64_t last_error;


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

void setPIDParameters(int32_t p,int32_t i,int32_t d)
{
    P = p;
    I = i;
    D = d;

    reset_pid();
}

void reset_pid()
{
    integral = 0;
    last_error = 0;
}

void init_pid()
{
    P = 1000;
    I = 0;
    D = 0;

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