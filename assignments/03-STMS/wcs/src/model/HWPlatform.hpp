#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "Arduino.h"
#include "config.hpp"
#include "events/EventQueue.hpp"
#include "devices/api/AbstractDevice.hpp"
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
  Button *pButton;
  Valve *pValve;
  Lcd *pLcd;
  Pot *pPot;
};

#endif