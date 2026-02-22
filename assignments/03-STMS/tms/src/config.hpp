#ifndef __APP_CONFIG__
#define __APP_CONFIG__

#include "Arduino.h"

// Conversion constants
#define FROM_MS_TO_S                0.001f
#define FROM_S_TO_MS                1000l
#define FROM_US_TO_S                0.000001f

#define TRIG_PIN                    // !!!!!
#define ECHO_PIN                    // !!!!!
#define MAX_TIME                    1 * FROM_S_TO_MS

#define MQTT_SERVER                 "localhost"
#define MQTT_PORT                   8883
#define TIMEOUT_MQTT_RECONNECTION   5000
#define MQTT_MSG_BUFFER_SIZE        50
#define MQTT_TOPIC                  "tank/water-level"

#endif