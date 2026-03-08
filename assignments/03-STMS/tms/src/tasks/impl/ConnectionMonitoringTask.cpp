#include "tasks/api/ConnectionMonitoringTask.hpp"

ConnectionMonitoringTask::ConnectionMonitoringTask(Context *pContext) :
    pTaskState(new StateHolder<ConnectionState>(WIFI_CONNECTING)),   
    pContext(pContext)
{
    this->setState(WIFI_CONNECTING);
}

void ConnectionMonitoringTask::tick()
{
    for(;;) {
        ConnectionState currentState = pTaskState->getState();
        switch (currentState)
        {
            case WIFI_CONNECTING:
            logger.log("WIFI Connecting state", "ConnectionMonitoringTask");
            while (!pContext->isWiFiOK) 
            {
                pContext->isWiFiOK = connectionProvider.wifiConnect();
                vTaskDelay(pdMS_TO_TICKS(10 * FROM_S_TO_MS));
            }
            logger.log("Is network ok? " + String(pContext->isWiFiOK), "ConnectionMonitoringTask");
            if (pContext->isWiFiOK)
            {
                this->setState(SERVER_CONNECTING);
            }
            break;
            
            case SERVER_CONNECTING:
            logger.log("MQTT Server connecting state", "ConnectionMonitoringTask");
            // pContext->isWiFiOK = connectionProvider.wifiIsConnected();
            // pContext->isMQTTOK = connectionProvider.mqttConnect();
            logger.log("can send data? " + String(pContext->isMQTTOK), "ConnectionMonitoringTask");
            if (!pContext->isWiFiOK)
            {
                this->setState(WIFI_CONNECTING);
                return;
            }
            if (pContext->isMQTTOK)
            {
                this->setState(CONNECTION_ENABLED);
            }
            break;
            
            case CONNECTION_ENABLED:
            logger.log("Connection Enabled state", "ConnectionMonitoringTask");
            pContext->isWiFiOK = connectionProvider.wifiIsConnected();
            pContext->isMQTTOK = connectionProvider.mqttIsConnected();
            if (!pContext->isWiFiOK)
            {
                this->setState(WIFI_CONNECTING);
            }
            if (!pContext->isMQTTOK)
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