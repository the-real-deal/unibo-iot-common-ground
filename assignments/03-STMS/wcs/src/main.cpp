#include <Arduino.h>
#include <kernel/MsgService.hpp>
#include <kernel/AsyncFSM.hpp>
#include <events/EventQueue.hpp>
#include <events/EventPublisher.hpp>
#include <events/PotEvent.hpp>
#include <events/SerialEvent.hpp>
#include <model/HWPlatform.hpp>
#include <devices/config/config.hpp>
#include <config.hpp>

AsyncFSM *asyncFSM;
HWPlatform *pHWPlatform;
EventPublisher *mainPublisher;

void setup()
{
  sharedQueue.init();
  msgService.init();
  pHWPlatform = new HWPlatform();
  asyncFSM = new AsyncFSM(
      pHWPlatform->getOperatorLCD(),
      pHWPlatform->getPotentiometer(),
      pHWPlatform->getValve());
  mainPublisher = new EventPublisher();
}

void loop()
{
  // Potentiometer publisher
  if (pHWPlatform->getPotentiometer()->hasChanged())
  {
    int rawOpening = pHWPlatform->getPotentiometer()->getValue();
    mainPublisher->publish(new PotEvent(rawOpening));
  }

  // Serial publisher - via polling 
  // (commented because serial event is indeed an EventPublisher)
  // if (msgService.isMsgAvailable())
  // {
  //   Msg *msg = msgService.receiveMsg();
  //   mainPublisher->publish(new SerialEvent(msg));
  // }

  delay(100);
  asyncFSM->checkAndProcessEvent();
}