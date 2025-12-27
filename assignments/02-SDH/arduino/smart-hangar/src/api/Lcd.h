#ifndef __LCD__
#define __LCD__

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

/* Lcd Class:
*  Interface for an LCD display connected via I2C.
*/
class Lcd {
public:
  Lcd();
  ~Lcd();
  void init();
  void clear();
  void print(const String& text);
  void print(const String& text, int row, int col);
    
private:
  LiquidCrystal_I2C display; 
};

#endif