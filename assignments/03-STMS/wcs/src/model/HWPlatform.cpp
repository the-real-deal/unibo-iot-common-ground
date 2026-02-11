#include "model/HWPlatform.hpp"

void wakeUp() {}

HWPlatform::HWPlatform()
{
  pModeToggleButton = new Button(BUTTON_PIN);
  pServoValve = new Valve(VALVE_PIN);
  pOperatorLCD = new Lcd();
  pOperatorLCD->init();
}

Button *HWPlatform::getModeToggleButton() { return this->pModeToggleButton; }
Valve *HWPlatform::getValve() { return this->pServoValve; }
Lcd *HWPlatform::getOperatorLCD() { return this->pOperatorLCD; }
