#include <Arduino.h>
#include <kernel/MsgService.hpp>
#include <kernel/AsyncFSM.hpp>
#include <events/EventQueue.hpp>
#include <model/HWPlatform.hpp>
#include <devices/config/config.hpp>
#include <config.hpp>

AsyncFSM* asyncFSM;
HWPlatform* pHWPlatform;
EventQueue *sharedQueue;
MsgServiceClass *msgService;

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
  asyncFSM->processEvent();
}