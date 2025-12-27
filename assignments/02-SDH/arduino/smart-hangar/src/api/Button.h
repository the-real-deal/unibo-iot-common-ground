#ifndef __BUTTON__
#define __BUTTON__

#include "Arduino.h"

/* Button Class:
*  Interface for a digital button connected to a specified pin.
*/
class Button {
public:
  Button(int pin);
  bool isPressed();
    
private:
  const int pin;
};

#endif