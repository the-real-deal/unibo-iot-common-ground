#ifndef __CONNECTION_PROVIDER__
#define __CONNECTION_PROVIDER__

#include "ArduinoMqttClient.h"
#include "WiFi.h"
#include "secrets.hpp"
#include "config.hpp"
#ifdef __DEBUG__
#include "kernel/Logger.hpp"
#endif

class ConnectionProviderClass 
{
private:
    WiFiClient _wifiClient;
    MqttClient _mqttClient;
public:
    ConnectionProviderClass() : _mqttClient(_wifiClient) {}
    void init();
    bool wifiConnect();
    bool wifiIsConnected();
    bool mqttConnect();
    bool mqttIsConnected();
    void mqttSendMsg(float msg);
};

extern ConnectionProviderClass connectionProvider;
#endif