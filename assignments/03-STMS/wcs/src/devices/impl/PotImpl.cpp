#include "devices/api/Pot.hpp"

/*
 * Pot Constructor
 */
Pot::Pot(uint8_t pin, EventQueue *queue) : AbstractDevice(pin), EventPublisher(queue)
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