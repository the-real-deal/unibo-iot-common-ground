#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "SyncTask.hpp"

#define MAX_TASKS 50

class Scheduler {
  
  int basePeriod;
  int nTasks;
  SyncTask* taskList[MAX_TASKS];  

public:
  void init(int basePeriod);  
  virtual bool addTask(SyncTask* task);  
  virtual void schedule();
};

#endif