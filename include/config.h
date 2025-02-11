#pragma once

#define TEMPERATURE_SENSOR_PIN 26

#define TEMPERATURE_SENSOR_ID 0


#define TEMPERATURE_SELECTION_PIN 27 

#define TEMPERATURE_SELECTION_ID 1 

#define MAIN_LOOP_REFRESH_MS 25


// signal LEDs:

#define TEMPERATURE_INDICATOR 15

// zero cross input

#define ZEROCROSS_DETECTOR_PIN 17

// heater output

// #define HEATER_OUTPUT 16

// for debug purpuse we use signal LED
#define HEATER_OUTPUT TEMPERATURE_INDICATOR

// buttons 

#define START_BUTTON 18

#define CALIB_BUTTON 19

#define AUTOMATIC_MANUAL_BUTTON 20