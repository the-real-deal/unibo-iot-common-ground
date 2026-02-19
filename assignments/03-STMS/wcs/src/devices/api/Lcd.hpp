#ifndef __LCD__
#define __LCD__

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "devices/api/AbstractDevice.hpp"
#include "devices/config/config.hpp"

/* Lcd Class:
 *  Interface for an LCD display connected via I2C.
 */
class Lcd: public AbstractDevice
{
public:
  Lcd();
  ~Lcd();
  void init();
  void clear(bool restore = false);
  void print(const String &text, bool saveMsg = false);
  void print(const String &text, int row, int col);

private:
  LiquidCrystal_I2C display;
};

#endif