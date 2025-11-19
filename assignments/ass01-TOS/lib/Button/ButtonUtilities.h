#ifndef __BUTTON_UTILS__
#define __BUTTON_UTILS__

/*
 * Returns the index of the value `buttonPin` inside the array `pins` of `nrButtons` elements. 
 * If no index found, returns `-1`. 
 */
int indexOfButton(int *pins, const int nrButtons, const int buttonPin);

/*
 *  Returns the button pin whose value is `HIGH`. If none detected, returns `-1`.
 */
int detectButtonPressed(int *pins, const int nrButtons);

/*
 * Sets the pin `pin` to `INPUT` mode.
 */
void initButton(const int pin); 

/*
 * Sets all the `pinsCount` pins `pins` to `INPUT` mode.
 */
void initAllButtons(int *pins, const int pinsCount);

#endif