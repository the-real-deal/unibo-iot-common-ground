#include "model/ConnectionProvider.hpp"
#include "model/Context.hpp"
#include "kernel/Logger.hpp"

#include "kernel/SyncTask.hpp"
#include "kernel/HWPlatform.hpp"
#include "tasks/api/ConnectionMonitoringTask.hpp"
#include "tasks/api/DataSenderTask.hpp"
#include "tasks/api/HWSampleWaterLevelTask.hpp"
#include "tasks/api/LedsTask.hpp"

HWPlatform *pHW;
Context* pContext;

SyncTask* pConnectionTask;
SyncTask* pDataSenderTask;
SyncTask* pSampleTask;
SyncTask* pLedsTask;

const int STACK_DEPTH = 10000; 

TaskHandle_t t1 = NULL;
TaskHandle_t t2 = NULL;
TaskHandle_t t3 = NULL;
TaskHandle_t t4 = NULL;

void setup() 
{
    connectionProvider.init();
    pHW = new HWPlatform();
    pContext = new Context(false, 0.0f, false);
    // TODO: Study how to use RTOS task creation
    
    pDataSenderTask = new DataSenderTask(pContext);
    pDataSenderTask->init(SAMPLING_PERIOD);
    xTaskCreatePinnedToCore([](void *p){ pDataSenderTask->tick(); }, "DataSenderTask", STACK_DEPTH, NULL, 1, &t2, 0);

    pSampleTask = new HWSampleWaterLevelTask(
        pHW->getTankSonar(),
        pContext
    );
    pSampleTask->init(SAMPLING_PERIOD);
    xTaskCreatePinnedToCore([](void *p){ pSampleTask->tick(); }, "SampleTask", STACK_DEPTH, NULL, 1, &t3, 1);

    pLedsTask = new LedsTask(
        pContext, pHW->getOKLed(), pHW->getKOLed()
    );
    pLedsTask->init(LED_PERIOD);
    xTaskCreatePinnedToCore([](void *p){ pLedsTask->tick(); }, "LedsTask", STACK_DEPTH, NULL, 1, &t4, 1);
}

void loop() 
{   
    pContext->isWiFiOK = connectionProvider.wifiIsConnected();
    if (!pContext->isWiFiOK) 
    {
        connectionProvider.wifiConnect();
        while (!connectionProvider.wifiIsConnected()) 
        {
            logger.log(".");
            delay(500);
        }
        pContext->isWiFiOK = true;
        logger.log("Connected to WiFi!");
    }

    pContext->isMQTTOK = connectionProvider.mqttIsConnected();
    if (!pContext->isMQTTOK) 
    {
        logger.log("Connecting to MQTT...");
        connectionProvider.mqttConnect();
        while (!connectionProvider.mqttIsConnected()) 
        {
            logger.log(".");
            delay(500);
        }
        pContext->isMQTTOK = true;
        logger.log("Connected to MQTT!");
    }
}