#ifndef __BLINKING_TASK__
#define __BLINKING_TASK__

#include "kernel/Task.h"
#include "context/Context.h"
#include "devices/api/Led.h"
#include "kernel/Config.h"
#include <Arduino.h>

class BlinkingTask: public Task {

public:
  BlinkingTask(Led* pLed, Context* pContext); 

  //TO DO: controllare se il distruttore può limitarsi a spegnere tutto
  ~BlinkingTask();
  void tick();

private:  

  enum blinkingState{ IDLE, OFF, ON };
  void setState(blinkingState state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  blinkingState state;
  long stateTimestamp;
  bool justEntered;

  Led* pLed;
  Context* pContext;
};

#endif