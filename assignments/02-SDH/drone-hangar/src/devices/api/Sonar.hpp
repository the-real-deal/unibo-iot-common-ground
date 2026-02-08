#ifndef __SONAR__
#define __SONAR__
#include "Arduino.h"

constexpr float NO_OBJ_DETECTED = -1.0f;

/* Sonar Class:
*  Interface for a US sensor connected to echo and trigger pins.
*/
class Sonar{
public:  
  Sonar(int echoPin, int trigPin, long maxTime);
  float getDistance();
  void setTemperature(float temp);

private:
  float getSoundSpeed() const;
  const int echoPin, trigPin;
  const long timeOut;
  float temperature;
};

#endif