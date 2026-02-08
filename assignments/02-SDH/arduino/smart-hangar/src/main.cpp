#include <Arduino.h>
#include <devices/config/config.hpp>
#include <tasks/api/BlinkingTask.hpp>
#include <tasks/api/HangarMainTask.hpp>
#include <tasks/api/TemperatureMonitoringTask.hpp>
#include <tasks/api/HangarDoorTask.hpp>
#include <kernel/Scheduler.hpp>
#include <model/HWPlatform.hpp>
#include <model/Context.hpp>

// #define __TESTING_HW__

HWPlatform* pHWPlatform;
Context* pContext;

TemperatureMonitoringTask* pTempMonitorTask;
HangarMainTask* pHangarMainTask;
BlinkingTask* pBlinkingTask;
HangarDoorTask* pDoorTask;

Scheduler scheduler;

void setup() {
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
  pTempMonitorTask->init(-1);

  pHangarMainTask = new HangarMainTask(
    pHWPlatform->getOperatorLCD(),
    pHWPlatform->getL1(),
    pHWPlatform->getDPD(),
    pHWPlatform->getDDD(),
    pHWPlatform->getTempSensor(),
    pContext
  );
  pHangarMainTask->init(-1);

  pBlinkingTask = new BlinkingTask(
    pHWPlatform->getL2(),
    pContext
  );
  pBlinkingTask->init(500);
  
  pDoorTask = new HangarDoorTask(
    pHWPlatform->getDoor(), 
    pContext
  );
  pDoorTask->init(-1);
  
  // Scheduler setup
  scheduler.init(100);  // 100ms base period
  
  scheduler.addTask(pTempMonitorTask);
  scheduler.addTask(pHangarMainTask);
  scheduler.addTask(pBlinkingTask);
  scheduler.addTask(pTempMonitorTask);
}

void loop() {
  scheduler.schedule();
}
