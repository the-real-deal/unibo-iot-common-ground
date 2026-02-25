#include "HWPlatform.hpp"

HWPlatform::HWPlatform(EventQueue *queue) : 
    pButton(new Button(BUTTON_PIN, queue)),
    pValve(new Valve(VALVE_PIN)),
    pLcd(new Lcd()),
    pPot(new Pot(POTENTIOMETER_PIN, queue))
{
    pLcd->init();
}

Button *HWPlatform::getModeToggleButton() { return pButton; }
Valve *HWPlatform::getValve() { return pValve; }
Lcd *HWPlatform::getOperatorLCD() { return pLcd; }
Pot *HWPlatform::getPotentiometer() { return pPot; }