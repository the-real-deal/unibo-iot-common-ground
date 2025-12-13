#ifndef __PIR__
#define __PIR__

#include "Arduino.h"

class Pir {
public:
  Pir(int pin);
  bool isDetected();
    
private:
  int pin;
};

#endif