#include "api/Door.h"
#include "Arduino.h"
#include "config/Config.h"

Door::Door(int pin){
  this->pin = pin;
  this->state = false;
  motor.attach(pin);
  close();
}

void Door::open(){
  motor.write(DOOR_OPEN_POS);
  state = true;
}

void Door::close(){
  motor.write(DOOR_CLOSE_POS);
  state = false;
}

bool Door::isOpen(){
  return state;
}