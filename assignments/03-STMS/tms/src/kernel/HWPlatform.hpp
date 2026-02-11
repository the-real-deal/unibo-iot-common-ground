#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "Arduino.h"
#include "config.hpp"
#include "devices/api/Sonar.hpp"

class HWPlatform
{

public:
  HWPlatform();
  Sonar *getTankSonar();
private:
  Sonar *getTankSonar();
};

#endif