#ifndef __APP_CONFIG__
#define __APP_CONFIG__

#define RESET_PIN           2
#define L1_PIN              13
#define L2_PIN              14
#define L3_PIN              15
#define DOOR_PIN            9
#define DPD_PIN             -1 ///
#define DDD_PIN             -1 ///
#define TEMP_SENSOR_PIN     -1 ////

// Conversion constants
#define FROM_MS_TO_S        0.001f
#define FROM_S_TO_MS        1000l
#define FROM_US_TO_S        0.000001f

#define D1                  1       // Minimum distance for OPERATING state to start, is evaluated in metres
#define T1                  5       // Minimum second for OPERATING state to start, is evaluated in s

#define D2                  0.5     // Maximum distance for LANDING state to start, is evaluated in metres
#define T2                  3       // Maximum second for LANDING state to start, is evaluated in s

#define TEMP1               30    
#define TEMP2               5000  
#define T3                  10000l 
#define T4                  8000l

#endif