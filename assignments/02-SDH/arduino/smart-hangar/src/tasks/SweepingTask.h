#ifndef __SWEEPING_TASK__
#define __SWEEPING_TASK__

#include "kernel/Task.h"
#include "devices/ServoMotor.h"
#include "kernel/MsgService.h"
#include "context/Context.h"
#include <Arduino.h>

class SweepingTask: public Task {

public:
  SweepingTask(Button* pButton, ServoMotor* pMotor, Context* pContext); 
  void tick();

private:  
  void setState(int state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  enum { IDLE, STARTING, SWEEPING_FWD, SWEEPING_BWD, RESETTING } state;
  long stateTimestamp;
  bool justEntered;

  Msg* pmessage;
  ServoMotor* pMotor;
  /*mettere sensori di distanza --> controllo chiusura e apertura porta*/
  Context* pContext;

  int currentPos;
  bool toBeStopped;
};

#endif