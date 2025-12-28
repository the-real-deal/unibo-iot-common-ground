#ifndef __CONTEXT__
#define __CONTEXT__

class Context {

public:

  enum HangarSystemState {DRONE_INSIDE, TAKE_OFF, DRONE_OUT, LANDING, ALARM};
  enum AlarmSystemState {NORMAL_STATE, PREALARM, ALARM};

  Context();

  bool isAlarm();
  bool canFly();
  bool isInit();
  void setInit();
  HangarSystemState getHangarSystemState();
  void setHangarSystemState(HangarSystemState s);
  AlarmSystemState getAlarmSystemState();
  void setAlarmSystemState(AlarmSystemState s);
  void reset();

private:

  bool canFly;
  bool isAlarm;
  bool isInit;
  HangarSystemState hangarSystemState;
  AlarmSystemState alarmSystemState;
  
};

#endif