#ifndef __CONNECTION_PROVIDER__
#define __CONNECTION_PROVIDER__

#include "ArduinoMqttClient.h"
#include "WiFiNINA.h"
#include "secrets.hpp"

class ConnectionProvider 
{
private:
    WiFiClient _wifiClient;
    MqttClient _mqttClient;
public:
    void init();
    wl_status_t wifiConnect();
    wl_status_t wifiIsConnected();
    bool mqttConnect();
    bool mqttIsConnected();
    void mqttSendMsg();
};

extern ConnectionProvider connectionProvider;
#endif