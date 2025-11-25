#include "servo_motor.h"
#include "Arduino.h"

Motor::Motor(int pin) {
  this->pin = pin;
}

void Motor::on() {
  this->motor.attach(this->pin);
}

void Motor::off() {
  this->motor.detach();
}

void Motor::setAngle(int angle) {
  this->motor.write(angle);
}