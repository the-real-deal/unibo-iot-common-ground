#ifndef __POT__
#define __POT__

#include "Arduino.h"
#include "devices/api/AbstractDevice.hpp"
#include "devices/config/config.hpp"
#include "events/EventPublisher.hpp"

/* Potentiometer Class:
 *  Interface for a potentiometer connected to an analog pin.
 */
class Pot: public AbstractDevice
{
private:
  int lastVal = 0;
public:
  Pot(uint8_t pin);
  int getValue();
  bool hasChanged();
};

#endif