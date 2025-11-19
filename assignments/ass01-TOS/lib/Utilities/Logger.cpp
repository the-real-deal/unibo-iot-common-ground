#include "Logger.h"
#include "Arduino.h"

void log(const String& message)
{
    Serial.println("[" + String(millis()) + "] Logger: " + message);
}

void logInfo(const String& message)
{
    log(String("{INFO}") + message);
}

void logError(const String& error)
{
    log(String("{INFO}") + error);
}
