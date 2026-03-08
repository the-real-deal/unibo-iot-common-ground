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
    WiFi.mode(WIFI_STA);
    _mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

bool ConnectionProviderClass::wifiConnect()
{
    int connectionResult = WiFi.begin(WIFI_SSID, WIFI_PASS);
    return wifiIsConnected();
}

bool ConnectionProviderClass::wifiIsConnected()
{
    bool connected = WiFi.status() == wl_status_t::WL_CONNECTED;
    return connected;
}

bool ConnectionProviderClass::mqttConnect()
{
    bool success = _mqttClient.connect("ESP32 TMS");
    if (success) 
    {
        _mqttClient.subscribe(MQTT_TOPIC);
        _mqttClient.loop();
    }
    return success;
}

bool ConnectionProviderClass::mqttIsConnected()
{
    bool isConnected = _mqttClient.connected();
    return isConnected;
}

void ConnectionProviderClass::mqttSendMsg(float msg)
{
    char toSend[MQTT_MSG_BUFFER_SIZE];
    snprintf(toSend, MQTT_MSG_BUFFER_SIZE, "%.2f", msg);
    _mqttClient.publish(String(MQTT_TOPIC).c_str(), toSend);
}
