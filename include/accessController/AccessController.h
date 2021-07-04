#include "rfid/RFID.h"
#include "log/Log.h"
#include "model/User.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "repository/UserService.h"
#include "mutex/mutex.h"

#ifndef ACCESS_CONTROLLER_H
#define ACCESS_CONTROLLER_H

class AccessController : public Log
{
private:
  std::string name;
  void grant(User grantedUser);
  void deny(std::string id);
  void checkCardValidity(std::string id);
  static void waitForACard(void *arg);
public:
  AccessController();
  void init();
};

#endif