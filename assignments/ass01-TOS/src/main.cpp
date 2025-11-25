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

#include <Utilities.h>
#include <LEDUtilities.h>
#include <LCDUtilities.h>
#include <ButtonUtilities.h>
#include <POTUtilities.h>
#include <PrintingUtilities.h>
#include <Logger.h>
#include <Timer.h>
#include <config.h>

#include <string.h>

#define ARRAY_LENGTH(vect) (int)(sizeof(vect) / sizeof(vect[0]))
#define SECONDS_TO_MILLIS(seconds) seconds * 1000L
#define BOUNCING_DELAY 150
#define INITIAL_MAX_TIME_SECONDS SECONDS_TO_MILLIS(24)
#define SCORE_INCREMENT 100
#define PRINTING_DELAY 2500
#define SLEEP_DELAY_SECONDS SECONDS_TO_MILLIS(10)

int score;
int buttonPins[] = {BTN1, BTN2, BTN3, BTN4};
int ledPins[] = {L1, L2, L3, L4};
int sequence[] = {1, 2, 3, 4};
int currentDifficulty;

unsigned long currentMaxTimeSeconds; 
volatile int hitsNumber;

volatile bool shouldWelcome;
volatile bool shouldDisplayRoundStart;

volatile long lastInterruptTime = 0;

volatile State currentGameState;

void changeState(State newState)
{
  currentGameState = newState;
}

void startSleeping()
{ 
  logInfo("SLEEP FUNCTION CALLED!");
  displayMessage(&lcd, "Going to sleep...");
  turnOff(LS);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
}

void stopSleeping()
{
  sleep_disable();
  resetTimer();
  shouldWelcome = true;
}

bool checkSequence(int index, int button)
{
  logInfo("Checking button " + String(button) + " at index " + String(index) + " against sequence value " + String(sequence[index]));
  return sequence[index] == button;
}

void setNewRound(unsigned long roundTimeMS) 
{
  logInfo("Starting round with " + String(roundTimeMS) + "ms available.");
  logInfo("Difficulty set to " + String(currentDifficulty));
  setTimerPeriod(roundTimeMS);
  startTimer();

  hitsNumber = 0;
  shouldDisplayRoundStart = true;
  turnOffAllLeds(ledPins, ARRAY_LENGTH(ledPins));
  shuffle(sequence, ARRAY_LENGTH(sequence));
  logInfo("Sequence: " + String(intArrayToString(sequence, ARRAY_LENGTH(sequence))));
}

void backToStart() 
{
  shouldWelcome = true;
  resetTimer();
  setTimerPeriod(SLEEP_DELAY_SECONDS);
  turnOffAllLeds(ledPins, ARRAY_LENGTH(ledPins));
  changeState(started);
}

void setNewGame() 
{
  score = 0;
  shouldWelcome = true;
  currentMaxTimeSeconds = INITIAL_MAX_TIME_SECONDS - SECONDS_TO_MILLIS((currentDifficulty + 1));
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
      changeState(preparing);
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

  setTimerPeriod(SLEEP_DELAY_SECONDS);
  startTimer();

  currentGameState = started;
  shouldWelcome = true;
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
    if (hasTimeElapsed()) 
    {
      changeState(sleeping);
      return;
    }
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
  case preparing: 
  {
    setNewGame();
    changeState(playing);
  }
  case playing:
  {
    if (shouldDisplayRoundStart)
    {
      turnOff(LS);
      displayMessage(&lcd, "Go");
      delay(PRINTING_DELAY);
      printSequence(&lcd, sequence, ARRAY_LENGTH(sequence));
      delay(PRINTING_DELAY);
      shouldDisplayRoundStart = false;
    }

    if (hasTimeElapsed()) 
    {
      changeState(gameover_lost);
    }
    break;
  }
  case gameover_victory:
  {
    score += SCORE_INCREMENT;
    printGoodEnding(&lcd, score);
    delay(PRINTING_DELAY);
    currentMaxTimeSeconds -= SECONDS_TO_MILLIS((currentDifficulty + 1));
    setNewRound(currentMaxTimeSeconds);
    changeState(playing);
    break;
  }
  case gameover_lost:
  {
    turnOnFor(LS, 2);
    printBadEnding(&lcd, score);
    delay(SECONDS_TO_MILLIS(10));
    backToStart();
    break;
  }
  default:
    break;
  }
}
