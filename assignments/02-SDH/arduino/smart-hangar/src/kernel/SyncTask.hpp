#ifndef __TASK__
#define __TASK__

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

  void setCompleted(){
    completed = true;
    active = false;
  }

  bool isCompleted(){
    return completed;
  }

  bool isActive(){
    return active;
  }

  int getPeriod(){
    return myPeriod;
  }

  virtual void setActive(bool active){
    timeElapsed = 0;
    this->active = active;
  }
  
private:

  int myPeriod;
  int timeElapsed;
  bool active;
  bool completed;

};

#endif