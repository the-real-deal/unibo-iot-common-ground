/*#define led1 13
#define led2 12
#define led3 8
#define led4 7
int myPins[] = {led1, led2, led3, led4};

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i <= 3; i++) {
    pinMode(myPins[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i <= 3; i++) {
    digitalWrite(myPins[i], HIGH);
    delay(500);
    digitalWrite(myPins[i], LOW);
    delay(500);
  }
    for(int i = 2; i > 0; i--) {
    digitalWrite(myPins[i], HIGH);
    delay(500);
    digitalWrite(myPins[i], LOW);
    delay(500);
  }
}*/

#define led1 13
#define led2 12
#define led3 8
#define led4 7
int myPins[] = {led1, led2, led3, led4};
int current = 0;
int step = 1;

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i <= 3; i++) {
    pinMode(myPins[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(myPins[current], HIGH);
  delay(500);
  digitalWrite(myPins[current], LOW);
  delay(500);
  current +=step;
  if (current < 0 || current > 4) {
    step = -step;
  }
}
