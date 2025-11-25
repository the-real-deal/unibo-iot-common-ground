#define led 13
#define button 2
int state = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led, LOW);
  if (digitalRead(button) == HIGH && state == 0) {
    digitalWrite(led, HIGH);
    state = 1;
  } else if (digitalRead(button) == HIGH && state == 1) {
      digitalWrite(led, LOW);
      state = 0;
  }
}
