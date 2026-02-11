#ifndef __APP_CONFIG__
#define __APP_CONFIG__

#include "Arduino.h"

#define BUTTON_PIN          2
#define POTENTIOMETER_PIN   A0
#define VALVE_PIN           10
#define LCD_SDA             A4
#define LCD_SCL             A5

// Conversion constants
#define FROM_MS_TO_S        0.001f
#define FROM_S_TO_MS        1000l
#define FROM_US_TO_S        0.000001f

#endif