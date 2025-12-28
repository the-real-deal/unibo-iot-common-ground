#include "api/Door.h"
#include "Arduino.h"
#include "config/Config.h"

/*
* Door Constructor
*/
Door::Door(int pin) : pin(pin), state(false) {
  motor.attach(pin);
  close();
}

/*
* Open the door.
*/
void Door::open(){
  motor.write(DOOR_OPEN_POS);
  state = true;
}

/*
* Close the door.
*/
void Door::close(){
  motor.write(DOOR_CLOSE_POS);
  state = false;
}

/*
* Check if the door is open.
*/
bool Door::isOpen(){
  return state;
}