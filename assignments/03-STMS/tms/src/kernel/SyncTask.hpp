#ifndef __TASK__
#define __TASK__

#include "Arduino.h"
#include "WString.h"

class SyncTask {

public:
  SyncTask(){
    active = false;
  }

  /* periodic */
  virtual void init(int period){
    myPeriod = period;  
    active = true;
    timeElapsed = 0;
  }

  virtual void tick() = 0;

  bool updateAndCheckTime(int basePeriod){
    timeElapsed += basePeriod;
    if (timeElapsed >= myPeriod){
      timeElapsed = 0;
      return true;
    } else {
      return false; 
    }
  }

  int getPeriod(){
    return myPeriod;
  }

  virtual void setActive(bool active){
    timeElapsed = 0;
    this->active = active;
  }

  virtual bool isActive() {
    return this->active;
  }

  virtual long elapsedTimeInState() {
    return millis() - stateTimestamp;
  }

  virtual bool checkAndSetJustEntered() {
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
  }

  virtual char* getName() {
    return this->taskName;
  }
  
protected:

  int myPeriod;
  int timeElapsed;

  long stateTimestamp;
  bool active;
  bool completed;
  bool justEntered;
  char *taskName;
};

#endif