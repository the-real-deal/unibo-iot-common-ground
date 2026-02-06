#include "Context.hpp"

Context::Context(Context::DroneStates initialDroneState, HangarStates initialHangarState)
{
  this->pDroneState = new StateHolder<DroneStates>(initialDroneState);
  this->pHangarState = new StateHolder<HangarStates>(initialHangarState);
  new DoorState();
}

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