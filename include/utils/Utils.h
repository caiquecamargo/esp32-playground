#include <string>
#include "time.h"
#include "Arduino.h"

#ifndef UTILS_H
#define UTILS_H

class Utils
{
public:
  static std::string generateRandomId();
};

#endif
