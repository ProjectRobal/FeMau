#include <string.h>

#include "oled.h"

#include "heater.h"
#include "buttons.h"

#include "bitmaps.h"

#include "screen.h"

static uint8_t oled_buffer[SSD1306_BUF_LEN] = {0};

static volatile bool blink = false;

static void clear_buffer()
{
    memset(oled_buffer,0,SSD1306_BUF_LEN);
}

void init_screen()
{
    i2c_init(SSD1306_I2C,SSD1306_I2C_CLK*1000);

    gpio_set_function(SSD1306_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SSD1306_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SSD1306_SDA_PIN);
    gpio_pull_up(SSD1306_SCL_PIN);

    SSD1306_init();
}

void update_screen(int32_t temperature,int32_t target_temperature)
{
    clear_buffer();

    bool mode = check_mode();

    bool enabled = check_enabled();

    uint16_t power = get_heater_duty_cycle();

    temperature = temperature/1000;

    target_temperature = target_temperature/1000;

    FillRect(oled_buffer,117,0,127,10,blink);

    blink = !blink;

    if( blink )
    {

    }

    if(mode)
    {
        WriteChar(oled_buffer,0,0,'A');
    }
    else
    {
        WriteChar(oled_buffer,0,0,'M');   
    }


    DrawLine(oled_buffer,0,10,127,10,true);

    if( enabled )
    {

        DrawBitmap(oled_buffer,5,27,32,32,bitmap_heats);

        DrawBitmap(oled_buffer,90,27,32,32,bitmap_heats);
        // in automatic mode show target and current temperature
        if(mode)
        {

            DrawLine(oled_buffer,61,10,61,55,true);

            WriteString(oled_buffer,0,20,"T:");

            WriteString(oled_buffer,65,20,"C:");

            char temp_buffer[4] = {0};

            // convert number to text
            temp_buffer[0] = target_temperature/100 + 48;
            temp_buffer[1] = (target_temperature - (target_temperature/100)*100)/10 + 48;
            temp_buffer[2] = (target_temperature - (target_temperature/10)*10) + 48;

            WriteString(oled_buffer,15,20,temp_buffer);

            WriteString(oled_buffer,41,20,"C");

            temp_buffer[0] = temperature/100 + 48;
            temp_buffer[1] = (temperature - (temperature/100)*100)/10 + 48;
            temp_buffer[2] = (temperature - (temperature/10)*10) + 48;

            WriteString(oled_buffer,81,20,temp_buffer);

            WriteString(oled_buffer,106,20,"C");

        }
        else
        {
            char temp_buffer[4] = {0};

            temp_buffer[0] = temperature/100 + 48;
            temp_buffer[1] = (temperature - (temperature/100)*100)/10 + 48;
            temp_buffer[2] = (temperature - (temperature/10)*10) + 48;

            WriteString(oled_buffer,40,20,temp_buffer);

            WriteString(oled_buffer,66,20,"C");
        }


        uint8_t progress_bar_witdth = power/32;

        // draw power scale
        for(uint8_t i=0;i<4;i++)
        {
            DrawLine(oled_buffer,0,60+i,progress_bar_witdth,60+i,true);
        }

    }
    else
    {
        DrawBitmap(oled_buffer,48,15,32,32,bitmap_sleepy);
    }

    SSD1306_send_buf(oled_buffer,SSD1306_BUF_LEN);
}

