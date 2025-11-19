#include "PrintingUtilities.h"
#include "LCDUtilities.h"
#include "Utilities.h"
#include "Arduino.h"

#define __DEBUG_CONSOLE__

void printStart(LiquidCrystal_I2C *lcd)
{
    #ifndef __DEBUG_CONSOLE__
    displayMessage(lcd, "Welcome to TOS!\nPress B1 to start.");
    #endif
    #ifdef __DEBUG_CONSOLE__
    Serial.println("Welcome to TOS!\nPress B1 to start.");
    #endif
}

void printSequence(LiquidCrystal_I2C *lcd, int *sequence, int sequenceLength)
{
    char *sequenceInString = intArrayToString(sequence, sequenceLength, "-");
    #ifndef __DEBUG_CONSOLE__
    displayMessage(lcd, sequenceInString);
    #endif
    #ifdef __DEBUG_CONSOLE__
    Serial.println(sequenceInString);
    #endif
    free(sequenceInString);
}