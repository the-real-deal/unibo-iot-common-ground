#include "devices/api/Led.h"
#include "Arduino.h"

/*
 * Led Constructor
 */
Led::Led(int pin) : pin(pin) {
  this->state = false;
  pinMode(pin, OUTPUT);
}

/*
 * Switch the LED on.
 */
void Led::switchOn(){
  digitalWrite(pin, HIGH);
  state = true;
}

/*
 * Switch the LED off.
 */
void Led::switchOff(){
  digitalWrite(pin, LOW);
  state = false;
}

/*
 * Check if the LED is on.
 */
bool Led::isOn(){
  return state;
}