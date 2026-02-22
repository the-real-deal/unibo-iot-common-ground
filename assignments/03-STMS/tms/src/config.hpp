#ifndef __APP_CONFIG__
#define __APP_CONFIG__

#define __DEBUG__

// Conversion constants
#define FROM_MS_TO_S                0.001f
#define FROM_S_TO_MS                1000l
#define FROM_US_TO_S                0.000001f

#define OK_LED_PIN                  -1 // TODO: Define !!!!!
#define KO_LED_PIN                  -1 // TODO: Define !!!!!

#define TRIG_PIN                    -1 // TODO: Define !!!!!
#define ECHO_PIN                    -1 // TODO: Define !!!!!
#define MAX_TIME                    1 * FROM_S_TO_MS

#define MQTT_SERVER                 "localhost"
#define MQTT_PORT                   8883
#define TIMEOUT_MQTT_RECONNECTION   5000
#define MQTT_MSG_BUFFER_SIZE        50
#define MQTT_TOPIC                  "tank/water-level"

#endif