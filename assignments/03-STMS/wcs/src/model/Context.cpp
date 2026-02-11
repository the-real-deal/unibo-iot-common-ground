#include "model/Context.hpp"

Context::Context(SystemStates initialSystemState, InputMode initialInputMode) : 
  pGlobalOpeningLevel(new StateHolder<float>(0.0f)),
  pGlobalSystemState(new StateHolder<SystemStates>(initialSystemState)),
  pGlobalInputMode(new StateHolder<InputMode>(initialInputMode))
{ }