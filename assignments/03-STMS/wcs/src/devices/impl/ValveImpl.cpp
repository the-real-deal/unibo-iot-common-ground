#include "devices/api/Valve.hpp"

/*
* Door Constructor
*/
Valve::Valve(uint8_t pin) : AbstractDevice(pin) {
  motor.attach(pin);
}

void Valve::setOpening(float percentage) {
  int newOpening = VALVE_CLOSE_POS + (int)(percentage * (VALVE_OPEN_POS - VALVE_CLOSE_POS));
  motor.write(newOpening);
}