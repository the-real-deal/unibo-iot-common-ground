#ifndef __LCD_UTILS__
#define __LCD_UTILS__

#define MAX_LINES_IDX 3
#define MAX_LINES MAX_LINES_IDX + 1

#include <LiquidCrystal_I2C.h>
/*
    Print a string `message` on the given LCD Display `display`.
    If the string `message` contains a new line character `\n`, then the display goes to the next line.
    If the string `message` contains more than or equal to `MAX_LINES` new line characters `\n`, then the program aborts.
*/
void displayMessage(LiquidCrystal_I2C* display, const String& message);

/**
 * Initializes `display`.
 */
void initLCD(LiquidCrystal_I2C* display);

#endif