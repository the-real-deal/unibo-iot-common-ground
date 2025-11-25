int c;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  c=0;
}

void loop() {
  // put your main code here, to run repeatedly:
  c++;
  Serial.print("Hello world ");
  Serial.println(c);
  //delay(100);

}
