#ifndef __LED__
#define __LED__
#include "Arduino.h"

/* Led Class:
*  Interface for a Led connected to a specified pin.
*/
class Led{ 
public:
  Led(int pin);
  void switchOn();
  void switchOff();
  bool isOn();
  bool isOff();
    
private:
  const int pin;
  bool state; 
};

#endif