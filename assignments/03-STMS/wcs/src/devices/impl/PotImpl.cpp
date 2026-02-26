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
int Pot::getValue()
{
  return analogRead(pin);
}

bool Pot::hasChanged()
{
  int newVal = getValue();
  if (abs(newVal - lastVal) > DELTA_POT) {
    lastVal = newVal;
    return true;
  }
  return false;
}