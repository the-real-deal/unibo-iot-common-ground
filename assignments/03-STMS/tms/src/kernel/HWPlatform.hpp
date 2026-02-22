#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "Arduino.h"
#include "config.hpp"
#include "devices/api/Sonar.hpp"
#include "devices/api/Led.hpp"

class HWPlatform
{
public:
  HWPlatform();
  Sonar *getTankSonar();
  Led *getOKLed();
  Led *getKOLed();
private:
  Sonar *pTankSonar;
  Led *pOKLed;
  Led *pKOLed;
};

#endif