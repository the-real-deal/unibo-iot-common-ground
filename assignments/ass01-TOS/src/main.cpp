#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#define EI_ARDUINO_INTERRUPTED_PIN;
#include <EnableInterrupt.h>
#include <TimerOne.h>

#define LS 9
#define B1 8
#define B2 7
#define B3 4
#define B4 2
#define L1 13
#define L2 12
#define L3 11
#define L4 10

LiquidCrystal_I2C lcd(0x20,  16, 2);

enum state {IDLE, START, PLAYING, GAMEOVER, WINNING};
volatile state gameState = START;
int score = 0;
int currentIntensity;
int fadeAmount;
int button_pins[] = {B1,B2,B3,B4};
int size = sizeof(button_pins)/sizeof(button_pins[0]);
int sequence[] = {1,2,3,4};
volatile int index = 0;
int timeMaxS = 15;
bool shouldWelcome = true;
void setPins(){
  /*Led setUp*/
  pinMode(LS, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  
  /*Button setUp*/
  pinMode(B1, INPUT);
  pinMode(B2, INPUT);
  pinMode(B3, INPUT);
  pinMode(B4, INPUT);
}

void changeState(state newState){
  gameState = newState;
}

bool checkSequence(int index, int button){
  return sequence[index] == button;
} 

void fading(){
  analogWrite(LS, currentIntensity);
  currentIntensity += fadeAmount;
  Serial.println(currentIntensity);
  if(currentIntensity <= 0 || currentIntensity >= 255){
  	fadeAmount = -fadeAmount;
    Serial.println(fadeAmount);
  }
  delay(50);
}  

void printStart(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome to TOS!");
    lcd.setCursor(0, 1);
    lcd.print("Press B1 to Start");	
}

void printSequence(){
    lcd.clear();
    lcd.setCursor(0,0);
    for (int i = 0; i<size; i++){
      lcd.print(sequence[i]);
    }
}

//Shuffle function that permutates an array.
void shuffle(int* array){
  for (int i=(size-1); i>=0; i--){
    int j = rand() % i;
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

void step(){
  int value = arduinoInterruptedPin;
  bool progress = true;

  switch (value)
  {
  case B1:
    if(gameState == START){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Go!");

      Timer1.attachInterrupt(inactivityGameOver);
      Timer1.setPeriod(timeMaxS*1000000);

      changeState(PLAYING);
    }
    else{
      progress = checkSequence(index,1);
    }
    break;
  case B2:
    progress = checkSequence(index,2);
  break;
  case B3:
    progress = checkSequence(index,3);
  break;
  case B4:
    progress = checkSequence(index,4);
  break;
  default:
    break;
  }

  if (!progress){
    changeState(GAMEOVER);
  }

  if (index<3){
    index++;
  }
  else{
    index = 0;
  }
  
}

void suggestSleep(){
  changeState(IDLE);
}

void inactivityGameOver(){
  changeState(GAMEOVER);
}

void setup()
{
  setPins();
  lcd.init();
  lcd.backlight();
  
  /*For fading*/
  currentIntensity = 0;
  fadeAmount = 5;
  Serial.begin(9600);

  for (int i = 0; i<size;i++){
    enableInterrupt(button_pins[i],step,RISING);
  }
  Timer1.attachInterrupt(suggestSleep);
  Timer1.setPeriod(10*1000000);
  
    shuffle(sequence);
}

void loop()
{ 
  switch (gameState)
  {
  case START :
    if (shouldWelcome){
      printStart();
      shouldWelcome = false;
    }
    fading();
    break;
  case PLAYING:
    printSequence();
    break;
  case WINNING:
    score++;
    printVictory();
    shuffle(sequence);
    changeState(PLAYING);
    break;
  case GAMEOVER:
    printGameOver();
    delay(10000);
    changeState(START);
    break;
  case IDLE:
    goToSpleep();
    break;
  }
  
}
