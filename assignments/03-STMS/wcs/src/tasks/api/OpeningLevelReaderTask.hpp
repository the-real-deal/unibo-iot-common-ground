#ifndef __OPENING_LEVEL_TASK__
#define __OPENING_LEVEL_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "devices/api/Pot.hpp"
#include "devices/api/Valve.hpp"
#include "devices/config/config.hpp"
#include "kernel/MsgService.hpp"
#include "Arduino.h"

enum OpeningLevelReaderTaskStates
{
    READING,
};

class OpeningLevelReaderTask: public SyncTask {
public:
  OpeningLevelReaderTask(Pot* pPot, Valve* pValve, Context* pContext); 
  void tick();

private:  
  void setState(OpeningLevelReaderTaskStates state);
  
  Pot *pPot;
  Valve *pValve;
  StateHolder<OpeningLevelReaderTaskStates>* pTaskState;
  Context* pContext;
};
#endif