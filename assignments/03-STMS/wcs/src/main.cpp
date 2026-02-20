#include <Arduino.h>
#include <kernel/MsgService.hpp>
#include <kernel/AsyncFSM.hpp>
#include <events/EventQueue.hpp>
#include <model/HWPlatform.hpp>
#include <devices/config/config.hpp>
#include <config.hpp>

AsyncFSM* asyncFSM;
HWPlatform* pHWPlatform;
EventQueue* sharedQueue;
MsgServiceClass* msgService;

void setup() {
  sharedQueue = new EventQueue();
  
  msgService = new MsgServiceClass(sharedQueue);
  pHWPlatform = new HWPlatform(sharedQueue);
  asyncFSM = new AsyncFSM(
    pHWPlatform->getOperatorLCD(),
    pHWPlatform->getPotentiometer(),
    pHWPlatform->getValve(),
    sharedQueue
  );
}

void loop() {
  asyncFSM->checkAndProcessEvent();
  if (asyncFSM->state->getState() == SystemState::MANUAL) 
  {
    float rawOpening = pHWPlatform->getPotentiometer()->getValue();
    pHWPlatform->getValve()->setOpening(map(rawOpening, POT_MIN, POT_MAX, 0, 100), 0L, 100L);
  }
}