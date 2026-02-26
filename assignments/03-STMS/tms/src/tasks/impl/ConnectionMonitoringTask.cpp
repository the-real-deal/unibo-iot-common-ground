#include "tasks/api/ConnectionMonitoringTask.hpp"

ConnectionMonitoringTask::ConnectionMonitoringTask(Context *pContext) :
    pTaskState(new StateHolder<ConnectionState>(WIFI_CONNECTING)),   
    pContext(pContext)
{
    this->setState(WIFI_CONNECTING);
}

void ConnectionMonitoringTask::tick()
{

    for(;;){

        ConnectionState currentState = pTaskState->getState();
        switch (currentState)
        {
            case WIFI_CONNECTING:
            pContext->isNetworkOk = connectionProvider.wifiConnect();
            if (pContext->isNetworkOk)
            {
                this->setState(SERVER_CONNECTING);
            }
            break;
            
            case SERVER_CONNECTING:
            pContext->isNetworkOk = connectionProvider.wifiIsConnected();
            pContext->canSendData = connectionProvider.mqttConnect();
            if (!pContext->isNetworkOk)
            {
                this->setState(WIFI_CONNECTING);
                return;
            }
            if (pContext->canSendData)
            {
                this->setState(CONNECTION_ENABLED);
            }
            break;
            
            case CONNECTION_ENABLED:
            pContext->isNetworkOk = connectionProvider.wifiIsConnected();
            pContext->canSendData = connectionProvider.mqttIsConnected();
            if (!pContext->isNetworkOk)
            {
                this->setState(WIFI_CONNECTING);
            }
            if (!pContext->canSendData)
            {
                this->setState(SERVER_CONNECTING);
            }
            break;
        }
        vTaskDelay(this->getPeriod());
    }
}

void ConnectionMonitoringTask::setState(ConnectionState state)
{
    this->justEntered = true;
    this->pTaskState->setState(state);
    this->stateTimestamp = millis();
}