#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "Arduino.h"
#include "config.hpp"
#include "devices/api/Button.hpp"
#include "devices/api/Valve.hpp"
#include "devices/api/Lcd.hpp"
#include "devices/api/Pot.hpp"

class HWPlatform
{

public:
  HWPlatform();
  Button *getModeToggleButton();
  Valve *getValve();
  Lcd *getOperatorLCD();
  Pot *getPotentiometer();
private:
  Button *pModeToggleButton;
  Valve *pServoValve;
  Lcd *pOperatorLCD;
  Pot *pPot;
};

#endif