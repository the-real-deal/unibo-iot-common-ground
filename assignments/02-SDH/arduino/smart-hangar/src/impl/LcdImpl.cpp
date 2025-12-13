#include "api/Lcd.h"
#include "Arduino.h"

Lcd::Lcd(){
  display = new LiquidCrystal_I2C(0x27, 16, 2);
}

void Lcd::init(){
  display->init();
  display->backlight();
}

void Lcd::clear(){
  display->clear();
}

void Lcd::print(String text){
  display->clear();
  display->setCursor(0, 0);
  display->print(text);
}

void Lcd::print(String text, int row, int col){
  display->setCursor(col, row);
  display->print(text);
}