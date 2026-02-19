#include "devices/api/Valve.hpp"

/*
* Door Constructor
*/
Valve::Valve(uint8_t pin) : AbstractDevice(pin) {
  motor.attach(pin);
}

void Valve::setOpening(float angle, long min, long max) {
  int newOpening = map(
    (long)constrain(angle, min, max), 
    min, 
    max, 
    VALVE_CLOSE_POS, 
    VALVE_OPEN_POS
  );
  motor.write(newOpening);
}