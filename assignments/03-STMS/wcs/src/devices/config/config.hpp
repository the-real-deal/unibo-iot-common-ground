#ifndef __DEVICES_CONFIG_PARAMS__
#define __DEVICES_CONFIG_PARAMS__

#include "devices/api/ServoTimer2.hpp"

#define DELTA_BTN               100     // Bedouncing delta time for button
#define DELTA_POT               10      // delta value to reduce potentiometer uncertainty

#define LCD_I2C_ADDRESS         0x27

#define VALVE_OPEN_POS          MAX_PULSE_WIDTH // Door open position (90 degrees)
#define VALVE_CLOSE_POS         MIN_PULSE_WIDTH // Door close position (0 degrees)

#define RESOLUTION              10
#define POT_MIN                 0
#define POT_MAX                 (int)pow(2, RESOLUTION) - 1

#endif