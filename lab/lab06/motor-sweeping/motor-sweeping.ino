#include "servo_motor.h"
#define MOTOR_PIN 9
#define MAX_ANGLE 180

Motor* servoMotor;
int delta;
int position;

void setup() 
{
  servoMotor = new Motor(MOTOR_PIN);
  delta = 1;
  position = 0;
}

void loop() 
{
  servoMotor->on();
  for (int i = 0; i < MAX_ANGLE; i++) 
  {
    servoMotor->setAngle(position);
    delay(2);
    position += delta;
  }
  servoMotor->off();
  position -= delta;
  delta = -delta;
  delay(1000);
}
