#include "console.h"

#include <memory.h>


static char message_buffer[BUFFER_SIZE] = {0};

static volatile uint16_t message_iter = 0;

static char header[BUFFER_SIZE] = {0};

static char value[BUFFER_SIZE] = {0};


bool process_console_input(char c)
{

    message_buffer[message_iter++] = c;

    if( message_iter >= 64 )
    {
        message_iter = 0;
        return false;
    }

    // process on newline
    if( c == '\n' )
    {

        uint16_t separation_id = 0;

        while( message_buffer[separation_id++] != ':' && separation_id<BUFFER_SIZE );

        if( separation_id >= BUFFER_SIZE )
        {
            message_iter = 0;
            return false;
        }

        memset(header,0,BUFFER_SIZE);

        // get header
        strncpy(header,message_buffer,separation_id);

        for(uint16_t i=0;i<separation_id;i++)
        {
            header[i] = message_buffer[i];
        }
        
        memset(value,0,BUFFER_SIZE);
        
        for(uint16_t i=separation_id+1;i<message_iter;i++)
        {
            value[i] = message_buffer[i];
        }

        message_iter = 0;

        return true;
    }

    return false;

}

const char* console_get_value()
{
    return value;
}

const char* console_get_header()
{
    return header;
}