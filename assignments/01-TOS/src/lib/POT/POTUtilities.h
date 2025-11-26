#ifndef __POT_UTILS__
#define __POT_UTILS__

#define SCALE (1024/4)
/*
* Returns level of difficulty from 0 to 4 based on the potentiometer reading.
*/
int readDifficultyFromPOT(int pot_pin);

bool hasDifficultyChanged(int previousDifficulty, int currentDifficulty);
#endif
