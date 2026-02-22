#include "ConnectionProvider.hpp"

void ConnectionProvider::init()
{
}

wl_status_t ConnectionProvider::wifiConnect()
{
    return wl_status_t();
}

wl_status_t ConnectionProvider::wifiIsConnected()
{
    return wl_status_t();
}

bool ConnectionProvider::mqttConnect()
{
    return false;
}

bool ConnectionProvider::mqttIsConnected()
{
    return false;
}

void ConnectionProvider::mqttSendMsg()
{
}
