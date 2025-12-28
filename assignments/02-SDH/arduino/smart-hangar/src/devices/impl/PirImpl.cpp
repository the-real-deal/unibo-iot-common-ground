#include "devices/api/Pir.h"
#include "Arduino.h"

/*
* Pir Constructor
*/
Pir::Pir(int pin) : pin(pin) {
  pinMode(pin, INPUT);
}

/*
* Check if motion is detected.
*/
bool Pir::isDetected(){
  return digitalRead(pin) == HIGH;
}