#ifndef __SONAR__
#define __SONAR__

#include "Arduino.h"
#include "config.hpp"

/* Sonar Class:
*  Interface for a US sensor connected to echo and trigger pins.
*/
class Sonar{
public:  
  Sonar(int echoPin, int trigPin);
  float getDistance();

private:
  const int echoPin, trigPin;
};

#endif