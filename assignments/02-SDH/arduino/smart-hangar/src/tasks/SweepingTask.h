#ifndef __SWEEPING_TASK__
#define __SWEEPING_TASK__

#include "kernel/Task.h"
#include "devices/api/ServoTimer2.h"
#include "kernel/MsgService.h"
#include "context/Context.h"
#include <Arduino.h>

class SweepingTask: public Task {

public:
  SweepingTask(ServoTimer2* pMotor, Context* pContext); 
  void tick();

private:  
  enum State { IDLE, STARTING, SWEEPING_FWD, SWEEPING_BWD, RESETTING };
  State state;
  long stateTimestamp;
  bool justEntered;

  void setState(State newState);
  long elapsedTimeInState();
  void log(const String& msg);
  bool checkAndSetJustEntered();
  

  Msg* pmessage;
  ServoTimer2* pMotor;
  /*mettere sensori di distanza --> controllo chiusura e apertura porta*/
  Context* pContext;

  int currentPos;
  bool toBeStopped;
};

#endif