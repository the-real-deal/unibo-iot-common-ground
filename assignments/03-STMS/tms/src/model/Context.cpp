#include "model/Context.hpp"

#include "model/Context.hpp"

Context::Context(bool isMQTTOK, 
    float waterLevel, 
    bool isWiFiOK) : 
    isMQTTOK(isMQTTOK), waterLevel(waterLevel), isWiFiOK(isWiFiOK)
{ }