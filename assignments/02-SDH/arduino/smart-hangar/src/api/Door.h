#ifndef __DOOR__
#define __DOOR__

#include "Arduino.h"
#include "ServoTimer2.h"

class Door {
public:
  Door(int pin);
  void open();
  void close();
  bool isOpen();
    
private:
  int pin;
  bool state;
  ServoTimer2 motor;
};

#endif