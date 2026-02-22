#include "model/Context.hpp"

#include "model/Context.hpp"

Context::Context(bool canSendData, 
    float waterLevel, 
    bool isNetworkOk) : 
    canSendData(canSendData), waterLevel(waterLevel), isNetworkOk(isNetworkOk)
{ }