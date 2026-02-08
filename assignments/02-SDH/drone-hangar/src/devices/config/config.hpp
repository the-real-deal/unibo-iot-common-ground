#ifndef __DEVICES_CONFIG_PARAMS__
#define __DEVICES_CONFIG_PARAMS__

#define DELTA                   100     // Bedouncing delta time for button

#define LCD_I2C_ADDRESS         0x27

#define DOOR_OPEN_POS           2250     // Door open position (90 degrees)
#define DOOR_CLOSE_POS          750     // Door close position (0 degrees)

#define ADC_RESOLUTION          1024.0  // Resolution of analog to digital conversion
#define VCC                     5.0     // Voltage
#define TEMP_SENSOR_OFFSET      0.5
#define TEMP_SENSOR_SCALE       100.0

#define NUM_TEMPERATURE_SAMPLES 5       // Number of times that the temperature is evaluated for average calculation
#define MIN_TEMP                -40.0   // Default
#define MAX_TEMP                125.0   // Default

#endif