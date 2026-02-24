#include "devices/api/Valve.hpp"

/*
* Door Constructor
*/
Valve::Valve(uint8_t pin) : AbstractDevice(pin) {
  motor.attach(pin);
}

void Valve::setOpening(float angle) {
  int newOpening = VALVE_CLOSE_POS + (int)(angle * (VALVE_OPEN_POS - VALVE_CLOSE_POS));
  motor.write(newOpening);
}