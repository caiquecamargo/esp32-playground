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
  template<typename TInputIter>
  static std::string make_hex_string(TInputIter first, TInputIter last);
};

#endif

