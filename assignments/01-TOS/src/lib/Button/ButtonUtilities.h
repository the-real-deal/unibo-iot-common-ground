#ifndef __BUTTON_UTILS__
#define __BUTTON_UTILS__

/*
 * Sets the pin `pin` to `INPUT` mode.
 */
void initButton(const int pin); 

/*
 * Sets all the `pinsCount` pins `pins` to `INPUT` mode.
 */
void initAllButtons(int *pins, const int pinsCount);

#endif