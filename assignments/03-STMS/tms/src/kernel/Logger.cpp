#include "Logger.hpp"

Logger logger;

Logger::Logger() { Serial.begin(115200); }
Logger::~Logger() { }
void Logger::log(String msg, String level) 
{
    String out = "[LOG] " + msg;
    if (!level.isEmpty()) 
    {
        out = "[" + level + "] " + msg;
    }
    Serial.println(out); 
}