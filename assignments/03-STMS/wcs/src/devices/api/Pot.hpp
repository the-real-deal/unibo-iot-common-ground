#ifndef __POT__
#define __POT__

#include "Arduino.h"

/* POtentiometer Class:
 *  Interface for a potentiometer connected to an analog pin.
 */
class Pot
{
public:
  Pot(int pin);
  float getValue();

private:
  const int pin;
};

#endif