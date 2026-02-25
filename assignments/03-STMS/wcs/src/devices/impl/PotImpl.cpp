#include "devices/api/Pot.hpp"

/*
 * Pot Constructor
 */
Pot::Pot(uint8_t pin) : AbstractDevice(pin)
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

bool Pot::hasChanged()
{
  float newVal = getValue();
  if (newVal != lastVal) {
    lastVal = newVal;
    return true;
  }
  return false;
}