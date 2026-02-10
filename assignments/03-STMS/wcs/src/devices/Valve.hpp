#ifndef __VALVE
#define __VALVE__

#include "Arduino.h"
#include "ServoTimer2.hpp"

/* Valve Class (servo motor):
*  Interface for a valve represented by a servo motor connected to a specified pin.
*/
class Valve {
public:
  Valve(int pin);
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