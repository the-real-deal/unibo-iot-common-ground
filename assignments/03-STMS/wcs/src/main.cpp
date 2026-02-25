#include <Arduino.h>
#include <kernel/MsgService.hpp>
#include <kernel/AsyncFSM.hpp>
#include <events/EventQueue.hpp>
#include <events/EventPublisher.hpp>
#include <events/PotEvent.hpp>
#include <model/HWPlatform.hpp>
#include <devices/config/config.hpp>
#include <config.hpp>

AsyncFSM* asyncFSM;
HWPlatform* pHWPlatform;
EventQueue* sharedQueue;
MsgServiceClass* msgService;
EventPublisher* potPublisher;

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

  potPublisher = new EventPublisher(sharedQueue);
}

void loop() {
  if (pHWPlatform->getPotentiometer()->hasChanged()) 
  {
    float rawOpening = pHWPlatform->getPotentiometer()->getValue();
    potPublisher->publish(new PotEvent(rawOpening));
  }
  delay(100);
  asyncFSM->checkAndProcessEvent();
}