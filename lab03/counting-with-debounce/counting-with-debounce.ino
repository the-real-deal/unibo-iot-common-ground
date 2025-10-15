#define BTN_PIN 2

// remember: the keyword `volatile` prevents some CPU automatic optimization 
// (like caching the variable value) — action needed to prevent race condition 
volatile int count = 0;
int prev = 0;

void setup() {
  Serial.begin(9600);
  // link an interrupt handler `increment()` to the change from LOW to HIGH of pin 2
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), increment, RISING);
}

void loop() {
  // disabling interrupts: action needed to prevent race conditions
  noInterrupts();
  int current = count;
  // re-enabling interrupts: remember to do it, because while interrupts are disabled the system is no longer responsive to events
  interrupts();

  // print the value if it has changed over time.
  // since `loop()` is called continuosly, we only want to print the value when the button is pressed
  if (current != prev) {
    prev = current;
    Serial.println("[BUTTON PRESSED]");
    Serial.println("Current count: " + String(count));
  }
}

void increment() {
  count++;
}
