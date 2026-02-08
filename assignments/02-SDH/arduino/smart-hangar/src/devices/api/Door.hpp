#ifndef __DOOR__
#define __DOOR__

#include "Arduino.h"
#include "ServoTimer2.hpp"

/* Door Class:
*  Interface for a door represented by a servo motor connected to a specified pin.
*/
class Door {
public:
  Door(int pin);
  void open();
  void close();
  bool isOpen();
  bool isClosed();
    
private:
  const int pin;
  bool state;
  ServoTimer2 motor;
};

#endif