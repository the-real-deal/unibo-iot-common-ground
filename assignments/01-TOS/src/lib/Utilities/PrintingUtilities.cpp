#include "lib/Utilities/PrintingUtilities.h"
#include "lib/LCD/LCDUtilities.h"
#include "lib/Utilities/Utilities.h"
#include "Arduino.h"

// Comment out the following define to debug the program with the terminal.
// #define __DEBUG_CONSOLE__

void printStart(LiquidCrystal_I2C *lcd, int difficulty)
{
    const String msg = "Welcome to TOS!\nPress B1 to start.\nDifficulty: " + String(difficulty);
    #ifndef __DEBUG_CONSOLE__
    displayMessage(lcd, msg);
    #endif
    #ifdef __DEBUG_CONSOLE__
    Serial.println(msg);
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

void printGoodEnding(LiquidCrystal_I2C *lcd, int score) 
{
    const String msg = "GOOD! Score: " + String(score);
    #ifndef __DEBUG_CONSOLE__
    displayMessage(lcd, msg);
    #endif
    #ifdef __DEBUG_CONSOLE__
    Serial.println(msg);
    #endif
}

void printBadEnding(LiquidCrystal_I2C *lcd, int score) 
{
    const String msg = "GAME OVER!\nFinal Score: " + String(score);
    #ifndef __DEBUG_CONSOLE__
    displayMessage(lcd, msg);
    #endif
    #ifdef __DEBUG_CONSOLE__
    Serial.println(msg);
    #endif
}