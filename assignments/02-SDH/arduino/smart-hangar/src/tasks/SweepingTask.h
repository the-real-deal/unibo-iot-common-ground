#ifndef __SWEEPING_TASK__
#define __SWEEPING_TASK__

#include "kernel/Task.h"
#include "devices/api/Door.h"
#include "devices/api/Pir.h"
#include "devices/api/Sonar.h"
#include "kernel/MsgService.h"
#include "context/Context.h"
#include <Arduino.h>

class SweepingTask: public Task {

public:
  SweepingTask(Door* pMotor, Context* pContext, Pir* pDistance, Sonar* pSonar); 
  void tick();

private: 
  enum doorState { IDLE, OPENING, CLOSING};
  void setState(doorState state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
 
  long stateTimestamp;
  bool justEntered;

  void setState(doorState newState);
  long elapsedTimeInState();
  void log(const String& msg);
  bool checkAndSetJustEntered();
  

  Msg* pmessage;
  Door* pMotor;
  Pir* pDistance; 
  Sonar* pSonar;
  Context* pContext;

  int currentPos;
  bool toBeStopped;
  doorState state;
};

#endif