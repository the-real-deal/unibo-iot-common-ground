#ifndef __LOGGER__
#define __LOGGER__

#include "Arduino.h"
#include "WString.h"

class Logger
{
public:
    Logger();
    ~Logger();
    void log(String level = "", String msg);
};

extern Logger logger;
#endif