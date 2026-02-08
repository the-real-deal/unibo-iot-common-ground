#include "model/Context.hpp"

Context::Context(Context::DroneStates initialDroneState, HangarStates initialHangarState) :
  pDroneState(new StateHolder<DroneStates>(initialDroneState)), 
  pHangarState(new StateHolder<HangarStates>(initialHangarState)),
  pDoorState(new DoorState())
{ }

Context::DoorState::DoorState()
{
  this->pDoorState = new StateHolder<bool>(false);
}

void Context::DoorState::openDoor()
{
  this->pDoorState->setState(true);
}

void Context::DoorState::closeDoor()
{
  this->pDoorState->setState(false);
}

bool Context::DoorState::isDoorOpen()
{
  return this->pDoorState->getState();
}