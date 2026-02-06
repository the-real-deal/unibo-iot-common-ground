#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "devices/api/Button.h"
#include "devices/api/Door.h"
#include "devices/api/Lcd.h"
#include "devices/api/Led.h"
#include "devices/api/Pir.h"
#include "devices/api/Sonar.h"
#include "devices/api/TempSensor.h"

#define LEDS_COUNT 3

class HWPlatform {

public:
  HWPlatform();

  Button* getResetButton();
  Led** getLeds();
  Led* getL1();
  Led* getL2();
  Led* getL3();
  Door* getDoor();
  Sonar* getDDD();
  Pir* getDPD();
  Lcd* getOperatorLCD();
  TempSensor* getTempSensor();
  
private:
  Button* pResetButton;
  Led* pLeds[LEDS_COUNT];
  Door* pServoDoor;
  Sonar* pDistanceDetector;
  Pir* pPresenceDetector;
  Lcd* pOperatorLCD;
  TempSensor* pTempSensor;

};

#endif