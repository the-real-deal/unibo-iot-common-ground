#include <Arduino.h>
#include <avr/sleep.h>
// Debugging tools — TODO: change if needed, not compatible with Arduino.h
// #include "avr8-stub.h"
// #include "app_api.h" // only needed with flash breakpoints

// See docs for explaination of this define: 
// https://github.com/GreyGnome/EnableInterrupt/wiki/Usage#determine-the-pin-that-was-interrupted
#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

#include <Utilities.h>
#include <LEDUtilities.h>
#include <LCDUtilities.h>
#include <ButtonUtilities.h>
#include <PrintingUtilities.h>
#include <config.h>

#include <string.h>

#define ARRAY_LENGTH(vect) (int)(sizeof(vect) / sizeof(vect[0]))
#define SECONDS_TO_MICRO(seconds) seconds * 1000000

int score = 0;
int buttonPins[] = {BTN1, BTN2, BTN3, BTN4};
int ledPins[] = {L1, L2, L3, L4};
int sequence[] = {1, 2, 3, 4};

volatile int hitsNumber = 0;

volatile bool shouldWelcome;
volatile bool shouldDisplaySequence;

volatile State currentGameState;

void changeState(State newState) 
{
  noInterrupts();
  currentGameState = newState;
  interrupts();
}

void suggestSleep() 
{
  changeState(sleeping);
}

void inactivityGameOver() 
{
  changeState(gameover_lost);
}

void startSleeping() 
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
}

void stopSleeping() 
{
  sleep_disable();
}

bool checkSequence(int index, int button) 
{
  return sequence[index] == button;
}

void step()
{
  // `arduinoInterruptedPin` contains the pin which triggered the interrupt.
  int buttonPressed = arduinoInterruptedPin;

  int buttonNumber;
  switch (buttonPressed)
  {
    // only the press of B1 can wake up the system from deep sleep.
    // so, if the button pressed is not B1 and the global state is IDLE or STARTED
    case BTN1:
      buttonNumber = 1;
      if (currentGameState == sleeping) 
      {
        stopSleeping();
        changeState(started);
        return;
      }
      if (currentGameState == started) 
      {
        displayMessage(&lcd, "Go");
        delay(2500);
        printSequence(&lcd, sequence, ARRAY_LENGTH(sequence));
        Timer1.detachInterrupt();
        Timer1.attachInterrupt(inactivityGameOver);
        Timer1.setPeriod(SECONDS_TO_MICRO(5));
        changeState(playing);
        return;
      }
      break;
    
    case BTN2:
      buttonNumber = 2;
      break;

    case BTN3:
      buttonNumber = 3;
      break;

    case BTN4:
      buttonNumber = 4;
      break;
    
    default:
      abort();
      break;
  }

  hitsNumber++;

  const int buttonIndex = buttonNumber - 1;
  const int correspondingLed = ledPins[buttonIndex];

  arduinoPinState == HIGH ? 
    turnOn(correspondingLed) : 
    turnOff(correspondingLed);
  
  if (!checkSequence(hitsNumber, buttonNumber) || hitsNumber > ARRAY_LENGTH(sequence)) 
  {
    changeState(gameover_lost);
    return;
  }

  /* Winning condition: the player turned on all the leds in the correct order. */
  if (hitsNumber == ARRAY_LENGTH(sequence)) 
  {
    changeState(gameover_victory);
    return;
  }
}

void setup()
{
  Serial.begin(9600);
  initAllButtons(buttonPins, ARRAY_LENGTH(buttonPins));
  initAllLeds(ledPins, ARRAY_LENGTH(ledPins));
  initLed(LS);
  initLCD(&lcd);

  for (int i = 0; i < ARRAY_LENGTH(buttonPins); i++)
  {
    // Before there was `attachInterrupt`, which is included in the Wiring framework.
    // The function `enableInterrupt` comes with the library `EnableInterrupt`
    enableInterrupt(buttonPins[i], step, CHANGE);
  }

  Timer1.initialize(SECONDS_TO_MICRO(10));
  Timer1.attachInterrupt(suggestSleep);

  shouldWelcome = true;
  shouldDisplaySequence = true;
  currentGameState = started;
}

void loop()
{
  switch (currentGameState)
  {
    case sleeping:
      startSleeping();
      break;
    case started:
      if (shouldWelcome) 
      {
        printStart(&lcd);
        shouldWelcome = false;
      }
      fadeLed(LS);
      break;
    case playing:
      if (shouldDisplaySequence) 
      {
        printSequence(&lcd, sequence, ARRAY_LENGTH(sequence));
        shouldDisplaySequence = false;
      } 
      break;
    default:
      break;
  }
}
