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

int score = 0;
int buttonPins[] = {BTN1, BTN2, BTN3, BTN4};
int ledPins[] = {L1, L2, L3, L4};
int sequence[] = {1, 2, 3, 4};
int difficulty = 0;

volatile int hitsNumber = 0;

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
  Serial.println("INTERRUPT CALLED");
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
  // only the press of B1 can wake up the system from deep sleep.
  // so, if the button pressed is not B1 and the global state is IDLE or STARTED
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
  const int indexInSequence = hitsNumber - 1;
  
  digitalRead(buttonPressed) == HIGH ? 
  turnOn(correspondingLed) : 
  turnOff(correspondingLed);
  
  logInfo("Button " + String(buttonNumber) + " pressed. Hit number: " + String(indexInSequence)); //!!!
  
  if (!checkSequence(indexInSequence, buttonNumber) || indexInSequence > ARRAY_LENGTH(sequence))
  {
    logInfo(String(!checkSequence(indexInSequence, buttonNumber)));
    logInfo(String(indexInSequence > ARRAY_LENGTH(sequence)));
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

void setNewRound() 
{
  hitsNumber = 0;
  shouldDisplayRoundStart = true;
  turnOffAllLeds(ledPins, ARRAY_LENGTH(ledPins));
  shuffle(sequence, ARRAY_LENGTH(sequence));
}

void setNewGame() 
{
  score = 0;
  shouldWelcome = true;
  setNewRound();
}

void turnOnErrorLed(int pin) 
{
    const int ERROR_LED_TIME = SECONDS_TO_MILLIS(2);
    turnOn(pin);
    delay(ERROR_LED_TIME);
    turnOff(pin);
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

  setNewGame();
  shouldWelcome = true;
  shouldDisplayRoundStart = true;
  currentGameState = started;
  shuffle(sequence, ARRAY_LENGTH(sequence));
}

void loop()
{
  switch (currentGameState)
  {
  case sleeping:
    startSleeping();
    break;
  case started:
    fadeLed(LS);
    if (hasDifficultyChanged(difficulty, readDifficultyFromPOT(POT_PIN)) || shouldWelcome){
        if (shouldWelcome)
        {
          shouldWelcome = false;
        }
        difficulty = readDifficultyFromPOT(POT_PIN);
        printStart(&lcd, difficulty);
    }
    break;
  case playing:
    if (shouldDisplayRoundStart)
    {
      // Gameover timeout interrupt
      // Timer1.detachInterrupt();
      // Timer1.attachInterrupt(inactivityGameOver);
      // Timer1.setPeriod(SECONDS_TO_MICRO(5));
      turnOff(LS);
      displayMessage(&lcd, "Go");
      delay(2500);
      printSequence(&lcd, sequence, ARRAY_LENGTH(sequence));
      delay(2500);
      shouldDisplayRoundStart = false;
    }
    break;
  case gameover_victory:
    score += 100;
    printGoodEnding(&lcd, score);
    changeState(playing);
    setNewRound();
    break;
  case gameover_lost:
    turnOnErrorLed(LS);
    printBadEnding(&lcd, score);
    //delay(SECONDS_TO_MILLIS(10));
    changeState(started);
    setNewGame();
    break;
  default:
    break;
  }
}
