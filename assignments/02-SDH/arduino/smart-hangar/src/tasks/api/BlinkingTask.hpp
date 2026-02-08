#ifndef __BLINKING_TASK__
#define __BLINKING_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "devices/api/Led.hpp"
#include "Arduino.h"

enum BlinkingTaskStates { OFF, ON };
class BlinkingTask: public SyncTask {

public:
  BlinkingTask(Led* pLed, Context* pContext); 
  void tick();

private:  
  void setState(BlinkingTaskStates state);
  
  StateHolder<BlinkingTaskStates>* pTaskState;
  Context* pContext;
  Led* pLed;
};

#endif