#include <Arduino.h>
#include <kernel/Scheduler.hpp>
#include <kernel/MsgService.hpp>
#include <model/HWPlatform.hpp>
#include <model/Context.hpp>
#include <kernel/SyncTask.hpp>
#include <devices/config/config.hpp>
#include <config.hpp>
#include <tasks/api/ConnectionMonitoringTask.hpp>
#include <tasks/api/HWModeSwitcherTask.hpp>
#include <tasks/api/OpeningLevelReaderTask.hpp>
#include <tasks/api/SerialReaderTask.hpp>

HWPlatform* pHWPlatform;
Context* pContext;

SyncTask* pConnectionTask;
SyncTask* pHWModeTask;
SyncTask* pOpeningTask;
SyncTask* pSerialReaderTask;

Scheduler scheduler;

void setup() {
  MsgService.init();
  pHWPlatform = new HWPlatform();
  pContext = new Context(Context::SystemStates::AVAILABLE, Context::InputMode::MANUAL);
  
  // Task creation
  pConnectionTask = new ConnectionMonitoringTask(pContext);
  pConnectionTask->init(.02 * FROM_S_TO_MS);

  pHWModeTask = new HWModeSwitcherTask(
    pHWPlatform->getModeToggleButton(),
    pHWPlatform->getOperatorLCD(),
    pContext
  );
  pHWModeTask->init(.02 * FROM_S_TO_MS);

  pOpeningTask = new OpeningLevelReaderTask(
    pHWPlatform->getPotentiometer(),
    pHWPlatform->getValve(),
    pContext
  );
  pOpeningTask->init(.02 * FROM_S_TO_MS);

  pSerialReaderTask = new SerialReaderTask(
    pHWPlatform->getOperatorLCD(),
    pContext
  );
  pSerialReaderTask->init(.02 * FROM_S_TO_MS);

  // Scheduler setup
  scheduler.init(.02 * FROM_S_TO_MS);
  
  scheduler.addTask(pConnectionTask);
  scheduler.addTask(pHWModeTask);
  scheduler.addTask(pOpeningTask);
  scheduler.addTask(pSerialReaderTask);

  if (pContext->pGlobalInputMode->getState() == Context::InputMode::AUTOMATIC) {
    pHWPlatform->getOperatorLCD()->print("AUTOMATIC");
  } else if (pContext->pGlobalInputMode->getState() == Context::InputMode::MANUAL) {
    pHWPlatform->getOperatorLCD()->print("MANUAL");
  } else {
    pHWPlatform->getOperatorLCD()->print("UNCONNECTED");
  }
}

void loop() {
  scheduler.schedule();
}