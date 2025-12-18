#ifndef __TEMP_SENSOR__
#define __TEMP_SENSOR__

#include "Arduino.h"

class TempSensor {
public:
  TempSensor(int pin);
  float getTemperature();
    
private:
  int pin;
};

#endif