#define LED_PIN LED_BUILTIN

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

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
