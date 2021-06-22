#include "log/Log.h"

Log::Log(std::string className) : className(className)
{
}

Log::~Log()
{
}

void Log::log(std::string message) {
  Serial.printf("%s: %s\n", className.c_str(), message.c_str());
}

void Log::logS(std::string className, std::string message) {
  Serial.printf("%s: %s\n", className.c_str(), message.c_str());
}