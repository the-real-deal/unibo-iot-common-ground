#ifndef __LCD__
#define __LCD__

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

class Lcd {
public:
  Lcd();
  void init();
  void clear();
  void print(String text);
  void print(String text, int row, int col);
    
private:
  LiquidCrystal_I2C* display;
};

#endif