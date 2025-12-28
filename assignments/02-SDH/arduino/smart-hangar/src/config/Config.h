#ifndef __CONFIG__
#define __CONFIG__

#define DELTA 100 // Bedouncing delta time for button
#define DOOR_OPEN_POS 2250     // Door open position (90 degrees)
#define DOOR_CLOSE_POS 750     // Door close position (0 degrees)

#define ADC_RESOLUTION 1024.0
#define VCC 5.0
#define TEMP_SENSOR_OFFSET 0.5
#define TEMP_SENSOR_SCALE 100.0

#define NUM_TEMPERATURE_SAMPLES 5
#define MIN_TEMP -40.0
#define MAX_TEMP 125.0

#define FROM_MS_TO_S 0.001f
#define FROM_US_TO_S 0.000001f

#endif