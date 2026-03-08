#ifndef __CONTEXT__
#define __CONTEXT__

#include "model/StateHolder.hpp"

/**
 * Collector of global variables shared among tasks. 
 */
class Context {

public:
  // Shared variables among tasks: the ones you can see in the FSM diagram of the project.
  bool isMQTTOK;
  float waterLevel;
  bool isWiFiOK;

  // Constructor with initial values
  // (no value is given for the door state because it is supposed to start on closed position) 
  Context(bool canSendData, 
    float waterLevel, 
    bool isWiFiOK);
  
};

#endif