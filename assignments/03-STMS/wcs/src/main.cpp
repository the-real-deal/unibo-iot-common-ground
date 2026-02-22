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
    sharedQueue,
    msgService
  );
}

void loop() {
  asyncFSM->checkAndProcessEvent();
  delay(100);
  if (asyncFSM->state->getState() == SystemState::MANUAL) 
  {
    float rawOpening = pHWPlatform->getPotentiometer()->getValue();
    int newOpening = map(rawOpening, POT_MIN, POT_MAX, 0, 100);
    pHWPlatform->getValve()->setOpening(newOpening, 0L, 100L);
    msgService->sendMsg("VALVE:" + String((float)newOpening / 100));
  }
}