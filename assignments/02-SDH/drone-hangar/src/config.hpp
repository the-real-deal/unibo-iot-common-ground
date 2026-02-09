#ifndef __APP_CONFIG__
#define __APP_CONFIG__

#include "Arduino.h"

#define RESET_PIN           2
#define L1_PIN              10
#define L2_PIN              11
#define L3_PIN              12
#define DOOR_PIN            9
#define DPD_PIN             6
#define DDD_ECHO_PIN        8 
#define DDD_TRIG_PIN        7
#define TEMP_SENSOR_PIN     A0
#define LCD_SDA             A4
#define LCD_SCL             A5

// Conversion constants
#define FROM_MS_TO_S        0.001f
#define FROM_S_TO_MS        1000l
#define FROM_US_TO_S        0.000001f

#define D1                  10.0f   // Minimum distance for OPERATING state to start, is evaluated in metres
#define T1                  5.0f    // Minimum second for OPERATING state to start, is evaluated in s

#define D2                  5.0f    // Maximum distance for LANDING state to start, is evaluated in metres
#define T2                  5.0f    // Maximum second for LANDING state to start, is evaluated in s

#define TEMP1               29.0f   
#define TEMP2               30.0f  
#define T3                  5.0f 
#define T4                  5.0f

#endif