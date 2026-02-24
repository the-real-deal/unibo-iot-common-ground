#include "model/ConnectionProvider.hpp"
#include "model/Context.hpp"

#include "kernel/SyncTask.hpp"
#include "kernel/Scheduler.hpp"
#include "kernel/HWPlatform.hpp"
#include "tasks/api/ConnectionMonitoringTask.hpp"
#include "tasks/api/DataSenderTask.hpp"
#include "tasks/api/HWSampleWaterLevelTask.hpp"
#include "tasks/api/LedsTask.hpp"

HWPlatform *pHW;
Context* pContext;

Scheduler scheduler;
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
    int t = 1;
    // TODO: Study how to use RTOS task creation
    pConnectionTask = new ConnectionMonitoringTask(pContext);
    pConnectionTask->init(-1); // TODO: period
    xTaskCreate([](void *p){ pConnectionTask->tick(); }, "ConnectionMonitoringTask", STACK_DEPTH, NULL, 2, &t1);
    
    pDataSenderTask = new DataSenderTask(pContext);
    pDataSenderTask->init(-1); // TODO: period
    xTaskCreate([](void *p){ pDataSenderTask->tick(); }, "DataSenderTask", STACK_DEPTH, NULL, 1, &t2);

    pSampleTask = new HWSampleWaterLevelTask(
        pHW->getTankSonar(),
        pContext
    );
    pSampleTask->init(-1); // TODO: period
    xTaskCreate([](void *p){ pSampleTask->tick(); }, "SampleTask", STACK_DEPTH, NULL, 1, &t3);

    pLedsTask = new LedsTask(
        pContext, pHW->getOKLed(), pHW->getKOLed()
    );
    xTaskCreate([](void *p){ pLedsTask->tick(); }, "LedsTask", STACK_DEPTH, NULL, 1, &t4);

    // scheduler.addTask(pConnectionTask);
    // scheduler.addTask(pSampleTask);
    // scheduler.addTask(pDataSenderTask);
    // scheduler.addTask(pLedsTask);
    // scheduler.init(-1);
}

void loop() 
{
    // scheduler.schedule();
}