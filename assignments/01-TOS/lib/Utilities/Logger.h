#ifndef __LOGGER_IMPL__
#define __LOGGER_IMPL__

#include <Arduino.h>

/*
 * Displays `message` on the terminal.
 */
void logMessage(const String& message);

/*
 * Displays `message` on the terminal at information level.  
 */
void logInfo(const String& message);

/*
 * Displays `message` on the terminal at error level. 
 */
void logError(const String& error);
#endif