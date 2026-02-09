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
  void clear(bool restore = false);
  void print(const String& text, bool saveMsg = false);
  void print(const String& text, int row, int col);
    
private:
  LiquidCrystal_I2C display; 
};

#endif