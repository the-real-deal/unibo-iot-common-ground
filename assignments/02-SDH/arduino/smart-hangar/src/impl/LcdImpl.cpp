#include "api/Lcd.h"
#include "Arduino.h"

Lcd::Lcd() : display(0x27, 16, 2) {
}

Lcd::~Lcd(){
  display.noBacklight();
  display.clear();
}

void Lcd::init() {
  display.init();
  display.backlight();
  display.clear();
}

void Lcd::clear() {
  display.clear();
}

void Lcd::print(const String& text) {
  display.clear();
  display.setCursor(0, 0);
  display.print(text);
}

void Lcd::print(const String& text, int row, int col) {
    display.setCursor(col, row);
    display.print(text);
}