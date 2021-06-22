#include <string>
#include "Arduino.h"

#ifndef LOG_H
#define LOG_H

class Log
{
private:
  std::string className;
public:
  Log(std::string className);
  ~Log();
  void log(std::string message);
  static void logS(std::string className, std::string message);
};

#endif
