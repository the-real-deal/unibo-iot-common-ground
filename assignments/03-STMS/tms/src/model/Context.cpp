#include "model/Context.hpp"

#include "model/Context.hpp"

Context::Context(bool canSendData, 
    float waterLevel, 
    bool isNetworkOk) : 
    
  pGlobalOpeningLevel(new StateHolder<float>(0.0f)),
  pGlobalNetworkOk(new StateHolder<bool>(isNetworkOk)),
  pGlobalCanSendData(new StateHolder<bool>(canSendData))
{ }