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