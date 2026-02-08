#ifndef __CONTEXT__
#define __CONTEXT__

#include "model/StateHolder.hpp"

/**
 * Collector of global variables shared among tasks. 
 */
class Context {

public:
  // Possible values of hangarState and droneState
  enum DroneStates {REST, TAKING_OFF, OPERATING, LANDING};
  enum HangarStates {NORMAL, PRE_ALARM, ALARM};

  // Constructor with initial values
  // (no value is given for the door state because it is supposed to start on closed position) 
  Context(DroneStates droneState, HangarStates hangarState);

  // DoorState nested class acts like a wrapper for a generic boolean StateHolder;
  // It is used to give specific and explicit names to the possible
  // operations that can be applied to a boolean state (i.e. set(true), set(false), get())
  class DoorState {
    public:
    DoorState(); 
    void openDoor();
    void closeDoor();
    bool isDoorOpen();
    private:
    StateHolder<bool>* pDoorState;
  };
  
  // Shared variables among tasks: the ones you can see in the FSM diagram of the project.
  StateHolder<DroneStates>* pDroneState;
  StateHolder<HangarStates>* pHangarState;
  DoorState *pDoorState;
};

#endif