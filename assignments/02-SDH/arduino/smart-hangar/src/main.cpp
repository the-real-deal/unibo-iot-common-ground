#include <devices/config/Config.h>
#include <tasks/api/BlinkingTask.hpp>
#include <tasks/api/HangarMainTask.hpp>
#include <tasks/api/TempMonitoring.h>
#include <kernel/Scheduler.hpp>
#include <Arduino.h>
#include <model/HWPlatform.hpp>

// #define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
Context* pContext;

// Define pin constants
// SETTED RANDOMLY!!!
#define DOOR_PIN 1
#define LED_PIN 2
#define PIR_PIN 3
#define ECHO_PIN 4
#define TRIG_PIN 5
#define BTN_PIN 6
#define TEMP_PIN 7

Door* door;
Led* led;
Lcd* lcd;
Pir* pir;
Sonar* sonar;
Button* button;
TempSensor* temperature;
Context* context;

TempMonitoring* tempMonitor;

Scheduler scheduler;

void setup() {
    // Hardware initialization
    door = new Door(DOOR_PIN);
    led = new Led(LED_PIN);
    lcd = new Lcd();
    pir = new Pir(PIR_PIN);
    sonar = new Sonar(ECHO_PIN, TRIG_PIN, 30000);
    button = new Button(BTN_PIN);
    temperature = new TempSensor(TEMP_PIN);
    //context = new Context();
    
    // Task creation
    tempMonitor = new TempMonitoring(lcd, led, temperature, context, button);
    
    // Scheduler setup
    scheduler.init(100);  // 100ms base period
    
    // Add tasks (aperiodic, will be activated on demand)
    //tempMonitor->init();
    
    scheduler.addTask(tempMonitor);

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
