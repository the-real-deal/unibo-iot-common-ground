#include "devices/api/Valve.hpp"
#include "Arduino.h"
#include "devices/config/config.hpp"

/*
* Door Constructor
*/
Valve::Valve(int pin) : pin(pin) {
  motor.attach(pin);
}

void Valve::setOpening(int angle) {
  motor.write(constrain(angle, VALVE_CLOSE_POS, VALVE_OPEN_POS));
}