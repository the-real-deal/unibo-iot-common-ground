#include "tasks/api/ConnectionMonitoringTask.hpp"

void ConnectionMonitoringTask::init(int period) 
{
    SyncTask::init(period);
    WiFi.begin("NickolausenMoPho", "password");
    WiFi.setHostname("TMS - Tank Monitoring Subsystem");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
}

void ConnectionMonitoringTask::tick()
{
    ConnectionState currentState = pTaskState->getState();
    switch (currentState)
    {
    case WIFI_CONNECTING:
        pContext->isNetworkOk = (WiFi.begin()==WL_CONNECTED);
        if (pContext->isNetworkOk)
        {
            pTaskState->setState(SERVER_CONNECTING);
        }
        else
        {
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        break;

    case SERVER_CONNECTING:
        pContext->isNetworkOk = (WiFi.begin()==WL_CONNECTED);
        //pContext->canSendData = mqttClient.connect(broker,port);
        if (!pContext->isNetworkOk)
        {
            pTaskState->setState(WIFI_CONNECTING);
            return;
        }
        if(pContext->canSendData)
        {
            pTaskState->setState(CONNECTION_ENABLED);
        }
        break;

    case CONNECTION_ENABLED:
        pContext->isNetworkOk = (WiFi.begin()==WL_CONNECTED);
        pContext->canSendData = mqttClient.connected();
        if (!pContext->isNetworkOk)
        {
            pTaskState->setState(WIFI_CONNECTING);
        }
        if(!pContext->canSendData)
        {
            pTaskState->setState(SERVER_CONNECTING);
        }
        break;
    }
}