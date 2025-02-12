#include "console.h"

#include <memory.h>


static volatile uint16_t message_iter = 0;

static char header[CONSOLE_BUFFER_SIZE] = {0};

static char value[CONSOLE_BUFFER_SIZE] = {0};

static bool extract_header = true;


bool process_console_input(char c)
{

    if( message_iter == 0 )
    {
        if( extract_header )
        {
            memset(header,0,CONSOLE_BUFFER_SIZE);   
        }
        else
        {
            memset(value,0,CONSOLE_BUFFER_SIZE);
        }
    }

    if( extract_header )
    {
        // igonore white letters
        if( c < 32 || c > 126 )
        {
            return false;
        }

        if( c == ':' )
        {
            extract_header = false;
            message_iter = 0;
            return false;
        }

        header[message_iter++] = c;
    }
    else
    {
        if( c == '\n' || c == '\r' )
        {
            extract_header = true;
            message_iter = 0;
            return true;
        }
        value[message_iter++] = c;
    }

    if( message_iter >= CONSOLE_BUFFER_SIZE-1 )
    {
        message_iter = 0;

        if( extract_header )
        {
            memset(header,0,CONSOLE_BUFFER_SIZE);
        }
        else
        {
            memset(value,0,CONSOLE_BUFFER_SIZE);
        }
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