#ifndef __SERVO_MOTOR_IMPL__
#define __SERVO_MOTOR_IMPL__

#include <Servo.h>

class Motor
{
  public:
    Motor(int pin);

    void on();
    void off();
    void setAngle(int angle);

  private:
    int angle;
    int pin;
    Servo motor;
};

#endif