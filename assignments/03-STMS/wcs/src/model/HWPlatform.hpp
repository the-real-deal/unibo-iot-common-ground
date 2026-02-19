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

#define NUM_DEVICES 4

class HWPlatform
{

public:
  HWPlatform(EventQueue *queue) : lastDevice(0)
  {
    add(new Pot(POTENTIOMETER_PIN, queue));
    add(new Button(BUTTON_PIN, queue));
    add(new Valve(VALVE_PIN));
    add(new Lcd());
  }
  Button *getModeToggleButton();
  Valve *getValve();
  Lcd *getOperatorLCD();
  Pot *getPotentiometer();

private:
  int lastDevice;
  AbstractDevice *devices[NUM_DEVICES];

  void add(AbstractDevice *device);

  template <typename T>
  T get()
  {
    for (int i = 0; i <= lastDevice; i++)
    {
      T target = static_cast<T>(devices[i]);
      if (target != nullptr)
      {
        return target;
      }
    }
    return nullptr;
  }
};

#endif