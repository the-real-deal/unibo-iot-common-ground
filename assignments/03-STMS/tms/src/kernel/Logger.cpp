#include "Logger.hpp"

Logger::Logger() { Serial.begin(115200); }
Logger::~Logger() { }
void Logger::log(String level, String msg) 
{
    String out = "[LOG] " + msg;
    if (!level.isEmpty()) 
    {
        out = "[" + level + "] " + msg;
    }
    Serial.println(out); 
}