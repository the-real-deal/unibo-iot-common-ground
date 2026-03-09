#ifndef __CONNECTION_PROVIDER__
#define __CONNECTION_PROVIDER__

#include "PubSubClient.h"
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
    PubSubClient _mqttClient{_wifiClient};
public:
    ConnectionProviderClass() {}
    void init();
    bool wifiConnect();
    bool wifiIsConnected();
    bool mqttConnect();
    bool mqttIsConnected();
    void mqttSendMeasurement(float value);
};

extern ConnectionProviderClass connectionProvider;
#endif