#include "utils/Utils.h"

std::string Utils::generateRandomId() {
  srand(time(NULL));
  std::string randomId = String(rand() % 1000 + 1000).c_str();

  return randomId;
}