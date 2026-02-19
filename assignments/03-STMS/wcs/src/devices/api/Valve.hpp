#ifndef __VALVE__
#define __VALVE__

#include "Arduino.h"
#include "ServoTimer2.hpp"
#include "devices/api/AbstractDevice.hpp"
#include "devices/config/config.hpp"

/* Valve Class:
 *  Interface for a door represented by a servo motor connected to a specified pin.
 */
class Valve: public AbstractDevice
{
public:
  Valve(uint8_t pin);
  void setOpening(float angle, long min, long max);

private:
  ServoTimer2 motor;
};

#endif