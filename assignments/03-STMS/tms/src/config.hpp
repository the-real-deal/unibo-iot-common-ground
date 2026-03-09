#ifndef __APP_CONFIG__
#define __APP_CONFIG__

#define __DEBUG__

// Conversion constants
#define FROM_MS_TO_S                0.001f
#define FROM_S_TO_MS                1000l
#define FROM_US_TO_S                0.000001f

#define OK_LED_PIN                  26
#define KO_LED_PIN                  27

#define TRIG_PIN                    5
#define ECHO_PIN                    18

#define MQTT_SERVER                 "broker.mqtt-dashboard.com"
#define MQTT_PORT                   1883
#define TIMEOUT_MQTT_RECONNECTION   5000
#define MQTT_MSG_BUFFER_SIZE        50
#define MQTT_TOPIC                  "tank/water-level"

#define SAMPLING_PERIOD             pdMS_TO_TICKS(.5 * FROM_S_TO_MS)
#define LED_PERIOD                  pdMS_TO_TICKS(.5 * FROM_S_TO_MS)
#define CONNECTION_PERIOD           pdMS_TO_TICKS(100)

#endif