#include "Context.h"

Context::Context(){
  hangarSystemState = DRONE_INSIDE;
  alarmSystemState = NORMAL_STATE;
  isInit=true;
  canFly = true;
  isAlarm = false;
}

bool Context::getIsAlarm(){
  return this->isAlarm;
}

bool Context::getCanFly(){
  return this->canFly;
}

bool Context::getIsInit(){
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

void Context::setCanFly(bool canFly){
  canFly = canFly;
}