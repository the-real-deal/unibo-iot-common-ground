#include "model/HWPlatform.hpp"
#include "HWPlatform.hpp"

void wakeUp() {}

HWPlatform::HWPlatform()
{
  pPot = new Pot(POTENTIOMETER_PIN);
  pModeToggleButton = new Button(BUTTON_PIN);
  pServoValve = new Valve(VALVE_PIN);
  pOperatorLCD = new Lcd();
  pOperatorLCD->init();
}

Button *HWPlatform::getModeToggleButton() { return this->pModeToggleButton; }
Valve *HWPlatform::getValve() { return this->pServoValve; }
Lcd *HWPlatform::getOperatorLCD() { return this->pOperatorLCD; }
Pot *HWPlatform::getPotentiometer() { return this->pPot; }
