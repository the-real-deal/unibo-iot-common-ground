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
#include <POTUtilities.h>
#include <PrintingUtilities.h>
#include <Logger.h>
#include <config.h>

#include <string.h>

#define ARRAY_LENGTH(vect) (int)(sizeof(vect) / sizeof(vect[0]))
#define SECONDS_TO_MICRO(seconds) seconds * 1000000
#define SECONDS_TO_MILLIS(seconds) seconds * 1000
#define BOUNCING_DELAY 150
#define INITIAL_MAX_TIME_SECONDS 60
#define SCORE_INCREMENT 100

int score;
int buttonPins[] = {BTN1, BTN2, BTN3, BTN4};
int ledPins[] = {L1, L2, L3, L4};
int sequence[] = {1, 2, 3, 4};
int currentDifficulty;
int currentMaxTimeSeconds; 
volatile int hitsNumber;

volatile bool shouldWelcome;
volatile bool shouldDisplayRoundStart;

volatile long lastInterruptTime = 0;

volatile State currentGameState;

void changeState(State newState)
{
  currentGameState = newState;
}

void suggestSleep()
{
  logInfo("SLEEP INTERRUPT CALLED");
  displayMessage(&lcd, "Going to sleep...");
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
  logInfo("Checking button " + String(button) + " at index " + String(index) + " against sequence value " + String(sequence[index]));
  return sequence[index] == button;
}

void setNewRound(int roundTimeSeconds) 
{
  // Timer1.stop();
  // Timer1.detachInterrupt();
  // Timer1.attachInterrupt(inactivityGameOver);
  // Timer1.setPeriod(SECONDS_TO_MICRO(roundTimeSeconds));
  // Timer1.start();

  hitsNumber = 0;
  shouldDisplayRoundStart = true;
  turnOffAllLeds(ledPins, ARRAY_LENGTH(ledPins));
  shuffle(sequence, ARRAY_LENGTH(sequence));
  logInfo(intArrayToString(sequence, ARRAY_LENGTH(sequence)));
}

void setNewGame() 
{
  score = 0;
  shouldWelcome = true;
  currentMaxTimeSeconds = INITIAL_MAX_TIME_SECONDS / (currentDifficulty + 1);
  setNewRound(currentMaxTimeSeconds);
}

void step()
{
  long interruptTime = millis();
  // Debounce
  if (interruptTime - lastInterruptTime < BOUNCING_DELAY){
    return;
  }
  lastInterruptTime = interruptTime;
  // `arduinoInterruptedPin` contains the pin which triggered the interrupt.
  int buttonPressed = arduinoInterruptedPin;

  int buttonNumber;
  switch (buttonPressed)
  {
  case BTN1:
    buttonNumber = 1;
    if (currentGameState == sleeping)
    {
      stopSleeping();
      changeState(started);
      return;
    } else if (currentGameState == started)
    {
      changeState(playing);
      setNewGame();
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
  
  if (currentGameState != playing) 
  {
    return;
  }

  hitsNumber++;
  
  const int buttonIndex = buttonNumber - 1;
  const int correspondingLed = ledPins[buttonIndex];
  const int indexInSequence = hitsNumber - 1;

  turnOn(correspondingLed);
  logInfo("Button " + String(buttonNumber) + " pressed. Hit number: " + String(indexInSequence)); //!!!
  
  if (!checkSequence(indexInSequence, buttonNumber) || indexInSequence > ARRAY_LENGTH(sequence))
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
    enableInterrupt(buttonPins[i], step, RISING);
  }

  // Timer1.initialize(SECONDS_TO_MICRO(10));
  // Timer1.attachInterrupt(suggestSleep);
  // Timer1.start();

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
  {
    fadeLed(LS);
    const int newDifficulty = readDifficultyFromPOT(POT_PIN);
    if (hasDifficultyChanged(newDifficulty, currentDifficulty) || shouldWelcome) 
    {
      currentDifficulty = newDifficulty;
      if (shouldWelcome)
      {
        shouldWelcome = false;
      }
      printStart(&lcd, currentDifficulty);
    }
    break;
  }
  case playing:
  {
    if (shouldDisplayRoundStart)
    {
      // Gameover timeout interrupt
      turnOff(LS);
      displayMessage(&lcd, "Go");
      delay(2500);
      printSequence(&lcd, sequence, ARRAY_LENGTH(sequence));
      delay(2500);
      shouldDisplayRoundStart = false;
    }
    break;
  }
  case gameover_victory:
  {
    score += SCORE_INCREMENT;
    printGoodEnding(&lcd, score);
    changeState(playing);
    currentMaxTimeSeconds /= (currentDifficulty + 1);
    setNewRound(currentMaxTimeSeconds);
    break;
  }
  case gameover_lost:
  {
    turnOnFor(LS, 2);
    printBadEnding(&lcd, score);
    delay(SECONDS_TO_MILLIS(10));
    changeState(started);
    setNewGame();
    break;
  }
  default:
    break;
  }
}
