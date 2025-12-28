#include "Context.h"

Context::Context(){
  hangarSystemState = DRONE_INSIDE;
  alarmSystemState = NORMAL_STATE;
  isInit=true;
  canFly = true;
  isAlarm = false;
}

bool Context::isAlarm(){
  return this->isAlarm;
}

bool Context::canFly(){
  return this->canFly;
}

bool Context::isInit(){
  return this->isInit;
}

Context::HangarSystemState Context::getHangarSystemState(){
  return this->hangarSystemState;
}

Context::AlarmSystemState Context::getAlarmSystemState(){
  return this->alarmSystemState;
}

void Context::reset(){
  hangarSystemState = DRONE_INSIDE;
  alarmSystemState = NORMAL_STATE;
  canFly = true;
  isAlarm = false;
}

void Context::setAlarmSystemState(AlarmSystemState s){
  alarmSystemState = s;
}

void Context::setHangarSystemState(HangarSystemState s){
  hangarSystemState = s;
}

void Context::setInit(){
  isInit = !isInit;
}