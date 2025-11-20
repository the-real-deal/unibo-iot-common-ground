#ifndef __LED_UTILS__
#define __LED_UTILS__

/*
 * Fade led by `fadeAmount` amount (default value: 5) at pin number `pin`.
 */
void fadeLed(const int pin, const int fadeAmount);

/*
 * Fade led by a default amount (`DEFAULT_FADE_AMOUNT = 5`) at pin number `pin`.
 */
void fadeLed(const int pin);

/*
 * Sets the pin `pin` to `OUTPUT` mode.
 */
void initLed(const int pin); 

/*
 * Sets all the `pinsCount` pins `pins` to `OUTPUT` mode.
 */
void initAllLeds(int *pins, const int pinsCount);

/*
 * Perform a `digitalWrite(HIGH)` on pin `pin`.
 */
void turnOn(int pin);

/**
 * Turn and keep on a led at `pin` pin for `seconds` seconds. 
 */
void turnOnFor(int pin, int seconds);

/*
 * Perform a `digitalWrite(LOW)` on pin `pin`.
 */
void turnOff(int pin);

/*
* Perform a `digitalWrite(LOW)` on all the `pinsCount` pins `pins`.
*/
void turnOffAllLeds(int *pins, const int pinsCount);

#endif