#include "devices/api/Lcd.hpp"
#include "devices/config/config.hpp"
#include "Arduino.h"

/*
 * Lcd Constructor
 */
Lcd::Lcd() : display(LCD_I2C_ADDRESS, 16, 2) {
}

/*
 * Lcd Destructor
 */
Lcd::~Lcd(){
  display.noBacklight();
  display.clear();
}

/*
 * Initialize the LCD display.
 */
void Lcd::init() {
  display.init();
  display.backlight();
  display.clear();
}

/*
 * Clear the LCD display.
 */
void Lcd::clear() {
  display.clear();
}

/*
 * Print text on the LCD display.
 */
void Lcd::print(const String& text) {
  display.clear();
  display.setCursor(0, 0);
  display.print(text);
}

/*
 * Print text on the LCD display at specified row and column.
 */
void Lcd::print(const String& text, int row, int col) {
    display.setCursor(col, row);
    display.print(text);
}