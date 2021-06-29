#include "rfid/RFID.h"
#include "log/Log.h"
#include "model/User.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "repository/UserService.h"

class AccessController : public Log
{
private:
  RFID rfid;
  UserService userService;
  void grant(User grantedUser);
  void deny();
  void checkCardValidity();
  static void waitForACard(void *arg);
public:
  AccessController(RFID rfid);
  void init();
};