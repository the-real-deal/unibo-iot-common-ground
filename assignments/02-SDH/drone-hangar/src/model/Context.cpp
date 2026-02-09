#include "model/Context.hpp"

Context::Context(Context::DroneStates initialDroneState, HangarStates initialHangarState) :
  pSharedDroneState(new StateHolder<DroneStates>(initialDroneState)), 
  pSharedHangarState(new StateHolder<HangarStates>(initialHangarState)),
  pSharedDoorState(new DoorState())
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