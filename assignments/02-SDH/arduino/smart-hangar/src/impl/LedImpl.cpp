#include "api/Led.h"
#include "Arduino.h"

Led::Led(int pin) : pin(pin) {
  this->state = false;
  pinMode(pin, OUTPUT);
}

void Led::switchOn(){
  digitalWrite(pin, HIGH);
  state = true;
}

void Led::switchOff(){
  digitalWrite(pin, LOW);
  state = false;
}

bool Led::isOn(){
  return state;
}