#include "devices/api/Button.h"
#include "Arduino.h"
#include "devices/config/Config.h"


/*
* Button Constructor
*/
Button::Button(int pin) : pin(pin), lastPressedTime(0), justClicked(false){
  pinMode(pin, INPUT_PULLUP);
}

/*
* Check if the button is pressed with bouncing implemented.
*/
bool Button::isPressed(){
  int currentState = digitalRead(pin);
  long currentTime = millis();
  if (currentState == LOW && (currentTime - lastPressedTime > DELTA)) {
    lastPressedTime = currentTime;
    return true;
  }
  return false;
}