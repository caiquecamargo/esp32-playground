#include "utils/Utils.h"

std::string Utils::generateRandomId() {
  srand(time(NULL));
  std::string randomId = String(rand() % 1000 + 1000).c_str();

  return randomId; 
}

std::string Utils::makeHexString(byte* first, byte* last) {
  std::ostringstream ss;
  ss << std::hex << std::setfill('0') << std::uppercase;

  while (first != last) {
    ss << std::setw(2) << static_cast<int>(*first++);
    
    if (first != last)
      ss << " ";
  }

  return ss.str();
}