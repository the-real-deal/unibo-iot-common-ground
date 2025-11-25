#include "Logger.h"
#include "Arduino.h"

void logMessage(const String& message)
{
    Serial.println("[" + String(millis()) + "] " + message);
}

void logInfo(const String& message)
{
    logMessage(String("{INFO} ") + message);
}

void logError(const String& error)
{
    logMessage(String("{ERROR} ") + error);
}
