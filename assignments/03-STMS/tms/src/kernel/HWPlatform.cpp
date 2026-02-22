#include "HWPlatform.hpp"

void wakeUp() {}

HWPlatform::HWPlatform()
{
    pTankSonar = new Sonar(ECHO_PIN, TRIG_PIN, MAX_TIME);
    pOKLed = new Led(OK_LED_PIN);
    pKOLed = new Led(KO_LED_PIN);
}

Sonar *HWPlatform::getTankSonar() { return this->pTankSonar; }
Led *HWPlatform::getOKLed() { return this->pOKLed; }
Led *HWPlatform::getKOLed() { return this->pKOLed; }