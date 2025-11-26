#include "Logger.h"
#include "Arduino.h"

void logMessage(const String& message)
{
    const unsigned long now = millis();
    
    const unsigned long ms = now % 1000UL;
    const unsigned long totalSeconds = now / 1000UL;
    const unsigned long seconds = totalSeconds % 60UL; 
    const unsigned long minutes = (totalSeconds / 60UL) % 60UL; 
    const unsigned long hours = (totalSeconds / 3600UL) % 24UL;
    
    char nowAsString[32];
    snprintf(nowAsString, sizeof(nowAsString), "%02lu:%02lu:%02lu:%03lu", hours, minutes, seconds, ms);
    Serial.print("[");
    Serial.print(nowAsString);
    Serial.print("] ");
    Serial.println(message);
}

void logInfo(const String& message)
{
    logMessage(String("{INFO} ") + message);
}

void logError(const String& error)
{
    logMessage(String("{ERROR} ") + error);
}
