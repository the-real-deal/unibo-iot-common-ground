#include "HWPlatform.hpp"

void wakeUp() {}

HWPlatform::HWPlatform()
{
    pTankSonar = new Sonar(ECHO_PIN, TRIG_PIN, MAX_TIME);
}

Sonar *HWPlatform::getTankSonar() {
    return this->pTankSonar;
}
