#ifndef __TEMP_SENSOR__
#define __TEMP_SENSOR__

#include "Arduino.h"

/* TempSensor Class:
*  Interface for a temperature sensor connected to a specified pin.
*/
class TempSensor {
public:
  TempSensor(int pin);
  float getTemperature() const;
    
private:
  const int pin;
};

#endif