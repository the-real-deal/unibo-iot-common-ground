#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <EnableInterrupt.h>

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

enum {IDLE, START, PLAYING, GAMEOVER};
int score = 0;
bool start = true;
int currentIntensity;
int fadeAmount;
int button_pins[] = {B1,B2,B3,B4};
int size = sizeof(button_pins)/sizeof(button_pins[0]);
int sequence[] = {1,2,3,4};
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
  int value;
  for (int i = 0; i<size; i++){
    if (digitalRead(button_pins[i])==HIGH){
      value = button_pins[i];
      break;
    }
  }

  switch (value)
  {
  case B1:
    if(start){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Go!");
    }
    else{

    }
    break;
  case B2:
  break;
  case B3:
  break;
  case B4:
  break;
  default:
    break;
  }
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
    attachInterrupt(digitalPinToInterrupt(button_pins[i]),step,RISING);
  }

}

void loop()
{
  if (start){
    fading();
    printStart();
  }
  else{
    shuffle(sequence);
    printSequence();
  }
  
}
