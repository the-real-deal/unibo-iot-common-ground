#ifndef __APP_CONFIG__
#define __APP_CONFIG__

#define __DEBUG__

// Conversion constants
#define FROM_MS_TO_S                0.001f
#define FROM_S_TO_MS                1000l
#define FROM_US_TO_S                0.000001f

#define OK_LED_PIN                  -1 // TODO: Define !!!!!
#define KO_LED_PIN                  -1 // TODO: Define !!!!!

#define TRIG_PIN                    5
#define ECHO_PIN                    18
#define MAX_TIME                    1 * FROM_S_TO_MS

#define MQTT_SERVER                 "localhost"
#define MQTT_PORT                   8883
#define TIMEOUT_MQTT_RECONNECTION   5000
#define MQTT_MSG_BUFFER_SIZE        50
#define MQTT_TOPIC                  "tank/water-level"

#define SAMPLING_PERIOD             pdMS_TO_TICKS(2 * FROM_S_TO_MS)
#define LED_PERIOD                  pdMS_TO_TICKS(0.5 * FROM_S_TO_MS)
#define CONNECTION_PERIOD           pdMS_TO_TICKS(100)



#endif