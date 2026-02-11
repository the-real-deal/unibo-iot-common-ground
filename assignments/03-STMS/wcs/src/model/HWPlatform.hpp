#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "Arduino.h"
#include "config.hpp"
#include "devices/api/Button.hpp"
#include "devices/api/Valve.hpp"
#include "devices/api/Lcd.hpp"

#define LEDS_COUNT 3

class HWPlatform
{

public:
  HWPlatform();
  Button *getModeToggleButton();
  Valve *getValve();
  Lcd *getOperatorLCD();
  // TODO: missing pot!
private:
  Button *pModeToggleButton;
  Valve *pServoValve;
  Lcd *pOperatorLCD;
  // TODO: missing pot!
};

#endif