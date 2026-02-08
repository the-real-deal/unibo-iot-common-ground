#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "Arduino.h"
#include "config.hpp"
#include "devices/api/Button.hpp"
#include "devices/api/Door.hpp"
#include "devices/api/Lcd.hpp"
#include "devices/api/Led.hpp"
#include "devices/api/Pir.hpp"
#include "devices/api/Sonar.hpp"
#include "devices/api/TempSensor.hpp"

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