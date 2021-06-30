#include <string>
#include "time.h"
#include "Arduino.h"
#include <sstream>
#include <iomanip>

#ifndef UTILS_H
#define UTILS_H

class Utils
{
public:
  static std::string generateRandomId();
  static std::string makeHexString(byte* first, byte* last);
};

#endif

