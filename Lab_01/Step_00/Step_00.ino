#define LED_PIN 13

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, HIGH);
  Serial.println("ON");
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  Serial.println("OFF");
  delay(500);
}
