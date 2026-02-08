#include "model/HWPlatform.hpp"

void wakeUp(){}

HWPlatform::HWPlatform(){
  pResetButton = new Button(RESET_PIN);
  pLeds[0] = new Led(L1_PIN);
  pLeds[1] = new Led(L2_PIN);
  pLeds[2] = new Led(L3_PIN);
  pServoDoor = new Door(DOOR_PIN);
  // pDistanceDetector = new Sonar(mi servono i pin);
  pPresenceDetector = new Pir(DPD_PIN);
  pOperatorLCD = new Lcd();
  pTempSensor = new TempSensor(TEMP_SENSOR_PIN);
}

Button* HWPlatform::getResetButton() { return this->pResetButton; }
Led** HWPlatform::getLeds() { return this->pLeds; }
Led* HWPlatform::getL1() { return this->pLeds[0]; }
Led* HWPlatform::getL2() { return this->pLeds[1]; }
Led* HWPlatform::getL3() { return this->pLeds[2]; }
Door* HWPlatform::getDoor() { return this->pServoDoor; }
Sonar* HWPlatform::getDDD() { return this->pDistanceDetector; }
Pir* HWPlatform::getDPD() { return this->pPresenceDetector; }
Lcd* HWPlatform::getOperatorLCD() { return this->pOperatorLCD; }
TempSensor* HWPlatform::getTempSensor() { return this->pTempSensor; }
