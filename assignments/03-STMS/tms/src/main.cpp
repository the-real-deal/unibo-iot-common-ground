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

void setup() 
{
    connectionProvider.init();
    pHW = new HWPlatform();
    pContext = new Context(false, 0.0f, false);

    pConnectionTask = new ConnectionMonitoringTask(pContext);
    pConnectionTask->init(-1); // TODO: period

    pDataSenderTask = new DataSenderTask(pContext);
    pDataSenderTask->init(-1); // TODO: period

    pSampleTask = new HWSampleWaterLevelTask(
        pHW->getTankSonar(),
        pContext
    );
    pSampleTask->init(-1); // TODO: period

    pLedsTask = new LedsTask(
        pContext, pHW->getOKLed(), pHW->getKOLed()
    );
    pLedsTask->init(-1); // TODO: period

    scheduler.addTask(pConnectionTask);
    scheduler.addTask(pSampleTask);
    scheduler.addTask(pDataSenderTask);
    scheduler.addTask(pLedsTask);
    scheduler.init(-1); // TODO: period
}

void loop() 
{
    scheduler.schedule();
}