#include "HWPlatform.hpp"

HWPlatform::HWPlatform() : 
    pButton(new Button(BUTTON_PIN)),
    pValve(new Valve(VALVE_PIN)),
    pLcd(new Lcd()),
    pPot(new Pot(POTENTIOMETER_PIN))
{
    pLcd->init();
}

Button *HWPlatform::getModeToggleButton() { return pButton; }
Valve *HWPlatform::getValve() { return pValve; }
Lcd *HWPlatform::getOperatorLCD() { return pLcd; }
Pot *HWPlatform::getPotentiometer() { return pPot; }