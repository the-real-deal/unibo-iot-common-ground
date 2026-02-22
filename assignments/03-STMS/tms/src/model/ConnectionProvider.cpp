#include "model/ConnectionProvider.hpp"
#include "ConnectionProvider.hpp"

#ifdef __DEBUG__
void log(String msg) 
{
    logger.log(msg, "ConnectionProviderClass");
}
#endif

ConnectionProviderClass connectionProvider;

void ConnectionProviderClass::init()
{
    WiFi.setHostname("ESP32 - Tank Monitoring Subsystem");
    _mqttClient.setClient(_wifiClient);
    #ifdef __DEBUG__
    log("Connection provider set up!");
    #endif
}

bool ConnectionProviderClass::wifiConnect()
{
    int connectionResult = WiFi.begin(WIFI_SSID, WIFI_PASS);
    #ifdef __DEBUG__
    log(".:: Connecting to WiFi ::.");
    log("SSID: " + String(WIFI_SSID) + "; Password: " + String(WIFI_PASS));
    log("Success? " + String(connectionResult == wl_status_t::WL_CONNECTED));
    #endif
    return connectionResult == wl_status_t::WL_CONNECTED;
}

bool ConnectionProviderClass::wifiIsConnected()
{
    #ifdef __DEBUG__
    log(".:: WiFi connection status ::.");
    log("Connected? " + String(WiFi.status() == wl_status_t::WL_CONNECTED));
    #endif
    return WiFi.status() == wl_status_t::WL_CONNECTED;
}

bool ConnectionProviderClass::mqttConnect()
{
    #ifdef __DEBUG__
    log(".:: Connecting to MQTT ::.");
    log("Server: " + String(MQTT_SERVER) + "; Port: " + String(WIFI_PASS));
    #endif
    return _mqttClient.connect(MQTT_SERVER, MQTT_PORT);
}

bool ConnectionProviderClass::mqttIsConnected()
{
    bool isConnected = _mqttClient.connected();
    if (isConnected) 
    {
        _mqttClient.poll();
    } 
    #ifdef __DEBUG__
    log(".:: MQTT Connection status ::.");
    log("Connected? " + String(isConnected));
    #endif
    return isConnected;
}

void ConnectionProviderClass::mqttSendMsg(float msg)
{
    #ifdef __DEBUG__
    log(".:: Sending via MQTT ::.");
    log("- Topic: " + String(MQTT_TOPIC));
    log("- Message: " + String(msg));
    #endif
    _mqttClient.beginMessage(MQTT_TOPIC);
    _mqttClient.print(msg);
    _mqttClient.endMessage();
}
