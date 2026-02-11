#ifndef __VALVE__
#define __VALVE__

#include "Arduino.h"
#include "ServoTimer2.hpp"

/* Valve Class:
 *  Interface for a door represented by a servo motor connected to a specified pin.
 */
class Valve
{
public:
  Valve(int pin);
  void setOpening(int angle);

private:
  int pin;
  ServoTimer2 motor;
};

#endif