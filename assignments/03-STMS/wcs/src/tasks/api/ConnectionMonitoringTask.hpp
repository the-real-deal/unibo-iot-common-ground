#ifndef __CONN_TASK__
#define __CONN_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "Arduino.h"

enum ConnectionMonitoringTaskStates { RUNNING };
class ConnectionMonitoringTask: public SyncTask {

public:
  ConnectionMonitoringTask(Context* pContext); 
  void tick();

private:  
  void setState(ConnectionMonitoringTaskStates state);
  
  StateHolder<ConnectionMonitoringTaskStates>* pTaskState;
  Context* pContext;
};

#endif