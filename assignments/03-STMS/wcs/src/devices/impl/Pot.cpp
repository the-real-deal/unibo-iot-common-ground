#include "devices/api/Pot.hpp"
#include "Arduino.h"
#include "devices/config/config.hpp"

/*
 * Pot Constructor
 */
Pot::Pot(int pin) : pin(pin)
{
  pinMode(pin, INPUT);
}

/*
 * Check the current value of the potentiometer
 */
float Pot::getValue()
{
  return analogRead(pin);
}