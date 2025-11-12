#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

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

int score = 0;
int currentIntensity;
int fadeAmount;
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
    lcd.setCursor(0, 0);
    lcd.print("Welcome to TOS!");
    lcd.setCursor(0, 1);
    lcd.print("Press B1 to Start");	
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
}

void loop()
{
  fading();
  printStart();
  delay(1000);
}

