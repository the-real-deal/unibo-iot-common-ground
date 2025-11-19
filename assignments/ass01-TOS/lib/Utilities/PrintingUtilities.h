#ifndef __PRINTING_UTILITES__
#define __PRINTING_UTILITIES__

#include "LiquidCrystal_I2C.h"

/*
 * Prints the welcome message on the display `lcd`. 
 */
void printStart(LiquidCrystal_I2C *lcd);

/**
 * Prints the integer sequence `sequence` of `sequenceLength` elements onto the display `lcd`. 
 */
void printSequence(LiquidCrystal_I2C *lcd, int *sequence, int sequenceLength);
#endif