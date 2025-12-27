#ifndef __PIR__
#define __PIR__
#include "Arduino.h"

/* Pir Class:
*  Interface for a PIR motion sensor connected to a specified pin.
*/
class Pir {
public:
  Pir(int pin);
  bool isDetected();
    
private:
  const int pin;
};

#endif