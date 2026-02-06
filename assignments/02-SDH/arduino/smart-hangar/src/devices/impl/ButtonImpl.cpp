#include "devices/api/Button.h"
#include "Arduino.h"
#include "devices/config/Config.h"


/*
* Button Constructor
*/
Button::Button(int pin) : pin(pin) {
  pinMode(pin, INPUT_PULLUP);
}

/*
* Check if the button is pressed with bouncing implemented.
*/
bool Button::isPressed(){
  long currentTime = millis();
  if(currentTime - lastPressedTime <= DEBOUNCING_DELTA_TIME) {
    lastPressedTime = currentTime;
    return digitalRead(pin) == HIGH;
  }
  return false;
}