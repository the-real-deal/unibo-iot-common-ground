#include <./devices/config/Config.h>
#include <./tasks/api/BlinkingTask.h>
#include <./tasks/api/LedTask.h>
#include <./tasks/api/TakeOffTask.h>
#include <./tasks/api/LandingTask.h>
#include <./tasks/api/TempMonitoring.h>
#include <./kernel/Scheduler.h>
#include <Arduino.h>

// Define pin constants
//BUTTATI A CASO PER NON AVERE ERRORI NEL CODICE
#define DOOR_PIN 1
#define LED_PIN 2
#define PIR_PIN 3
#define ECHO_PIN 4
#define TRIG_PIN 5

Door* door;
Led* led;
Lcd* lcd;
Pir* pir;
Sonar* sonar;
Context* context;

TakeOffTask* takeOffTask;
LandingTask* landingTask;
//TempMonitoring* tempMonitor;

Scheduler scheduler;

void setup() {
    // Hardware initialization
    door = new Door(DOOR_PIN);
    led = new Led(LED_PIN);
    lcd = new Lcd();
    pir = new Pir(PIR_PIN);
    sonar = new Sonar(ECHO_PIN, TRIG_PIN, 30000);
    context = new Context();
    
    // Task creation
    takeOffTask = new TakeOffTask(lcd, led, door, context, sonar);
    landingTask = new LandingTask(lcd, led, door, context, pir, sonar);
    //tempMonitor = new TempMonitoring(lcd, led, context);
    
    // Scheduler setup
    scheduler.init(100);  // 100ms base period
    
    // Add tasks (aperiodic, will be activated on demand)
    takeOffTask->init();  // Aperiodic task
    landingTask->init();  // Aperiodic task
    //tempMonitor->init();
    
    scheduler.addTask(takeOffTask);
    scheduler.addTask(landingTask);
    //scheduler.addTask(tempMonitor);

  Serial.begin(9600);  // Initialize serial communication at 9600 baud
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');  // Read until newline
    Serial.print("Received: ");
    Serial.println(data);
  }
  scheduler.schedule();
}
