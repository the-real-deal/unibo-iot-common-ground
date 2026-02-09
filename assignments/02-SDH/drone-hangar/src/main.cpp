#include <Arduino.h>
#include <devices/config/config.hpp>
#include <tasks/api/BlinkingTask.hpp>
#include <kernel/SyncTask.hpp>
#include <tasks/api/HangarMainTask.hpp>
#include <tasks/api/TemperatureMonitoringTask.hpp>
#include <tasks/api/HangarDoorTask.hpp>
#include <kernel/Scheduler.hpp>
#include <kernel/MsgService.hpp>
#include <model/HWPlatform.hpp>
#include <model/Context.hpp>

// #define __TESTING_HW__

HWPlatform* pHWPlatform;
Context* pContext;

SyncTask* pTempMonitorTask;
SyncTask* pHangarMainTask;
SyncTask* pBlinkingTask;
SyncTask* pDoorTask;

Scheduler scheduler;

void setup() {
  MsgService.init();
  // Hardware initialization
  pHWPlatform = new HWPlatform();
  pContext = new Context(Context::DroneStates::REST, Context::HangarStates::NORMAL);
  
  // Task creation
  pTempMonitorTask = new TemperatureMonitoringTask(
    pHWPlatform->getOperatorLCD(), 
    pHWPlatform->getL3(), 
    pHWPlatform->getTempSensor(), 
    pHWPlatform->getResetButton(),
    pContext
  );
  pTempMonitorTask->init(0.5 * FROM_S_TO_MS);

  pHangarMainTask = new HangarMainTask(
    pHWPlatform->getOperatorLCD(),
    pHWPlatform->getL1(),
    pHWPlatform->getDPD(),
    pHWPlatform->getDDD(),
    pHWPlatform->getTempSensor(),
    pContext
  );
  pHangarMainTask->init(0.5 * FROM_S_TO_MS);

  pBlinkingTask = new BlinkingTask(
    pHWPlatform->getL2(),
    pContext
  );
  pBlinkingTask->init(0.5 * FROM_S_TO_MS);
  
  pDoorTask = new HangarDoorTask(
    pHWPlatform->getDoor(), 
    pContext
  );
  pDoorTask->init(0.5 * FROM_S_TO_MS);
  
  // Scheduler setup
  scheduler.init(0.5 * FROM_S_TO_MS);
  
  scheduler.addTask(pTempMonitorTask);
  scheduler.addTask(pHangarMainTask);
  scheduler.addTask(pBlinkingTask);
  scheduler.addTask(pDoorTask);
}

void loop() {
  scheduler.schedule();
}
