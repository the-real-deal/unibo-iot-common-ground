#include "ConnectionProvider.hpp"

void ConnectionProvider::init()
{
}

bool ConnectionProvider::wifiConnect()
{
    // wl_status_t::WL_CONNECTED
    return true;
}

bool ConnectionProvider::wifiIsConnected()
{
    // wl_status_t::WL_CONNECTED
    return true;
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
