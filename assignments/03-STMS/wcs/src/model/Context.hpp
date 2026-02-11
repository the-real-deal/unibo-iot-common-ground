#ifndef __CONTEXT__
#define __CONTEXT__

#include "model/StateHolder.hpp"

/**
 * Collector of global variables shared among tasks. 
 */
class Context {

public:
  enum SystemStates {NOT_AVAILABLE, AVAILABLE};
  enum InputMode {AUTOMATIC, MANUAL, DISCONNECTED};

  // Constructor with initial values
  // (no value is given for the door state because it is supposed to start on closed position) 
  Context(SystemStates initialSystemState, InputMode initialInputMode);
  
  // Shared variables among tasks: the ones you can see in the FSM diagram of the project.
  StateHolder<float>* pGlobalOpeningLevel;
  StateHolder<InputMode>* pGlobalInputMode;
  StateHolder<SystemStates>* pGlobalSystemState;
};

#endif