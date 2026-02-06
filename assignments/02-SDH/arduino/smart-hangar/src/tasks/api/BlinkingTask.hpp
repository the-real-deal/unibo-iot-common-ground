#ifndef __BLINKING_TASK__
#define __BLINKING_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "devices/api/Led.h"
#include "Arduino.h"

class BlinkingTask: public SyncTask {

public:
  BlinkingTask(Led* pLed, Context* pContext); 
  void tick();

private:  
  Led* pLed;
  enum BlinkingTaskState { OFF, ON } state;
  void setState(BlinkingTaskState state);
  long elapsedTimeInState();
  
  bool checkAndSetJustEntered();
  
  long stateTimestamp;
  bool justEntered;

  Context* pContext;
};

#endif