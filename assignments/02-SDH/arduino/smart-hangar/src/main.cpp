#include <./tasks/BlinkingTask.h>
#include <./tasks/DisplayTask.h>
#include <./tasks/LedTask.h>
#include <./tasks/SweepingTask.h>
#include <./tasks/TakeOffTask.h>
#include <Arduino.h>

void setup() {
/**    // Hardware initialization
    Door* door = new Door(DOOR_PIN);
    Led* led = new Led(LED_PIN);
    Lcd* lcd = new Lcd();
    Pir* pir = new Pir(PIR_PIN);
    Sonar* sonar = new Sonar(ECHO_PIN, TRIG_PIN, 30000);
    Context* context = new Context();
    
    // Task creation
    TakeOffTask* takeOffTask = new TakeOffTask(lcd, led, door, context, pir, sonar);
    LandingTask* landingTask = new LandingTask(lcd, led, door, context, pir, sonar);
    
    // Scheduler setup
    Scheduler scheduler;
    scheduler.init(100);  // 100ms base period
    
    // Add tasks (aperiodic, will be activated on demand)
    takeOffTask->init();  // Aperiodic task
    landingTask->init();  // Aperiodic task
    
    scheduler.addTask(takeOffTask);
    scheduler.addTask(landingTask);
*/

  Serial.begin(9600);  // Initialize serial communication at 9600 baud
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');  // Read until newline
    Serial.print("Received: ");
    Serial.println(data);
  }
}
