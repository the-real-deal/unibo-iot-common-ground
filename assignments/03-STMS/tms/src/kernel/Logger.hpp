#ifndef __LOGGER__
#define __LOGGER__

#include "Arduino.h"
#include "WString.h"

class Logger
{
public:
    Logger();
    ~Logger();
    void log(String msg, String level = "");
};

extern Logger logger;
#endif