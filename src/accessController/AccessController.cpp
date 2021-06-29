#include "accessControlller/AccessController.h"

AccessController::AccessController(RFID rfid) :
  Log("AccessController"),
  rfid(rfid)
  {};

void AccessController::grant(User grantedUser) {
  log("Granting access to " + grantedUser.name + " with cardId " + grantedUser.cardId);
};

void AccessController::deny() {
  log("Access denied to card " + rfid.cardId);
};

void AccessController::checkCardValidity() {
  if (userService.findById(rfid.cardId)) {
    userService.resultSet.size() > 0 ? grant(userService.resultSet[0]) : deny(); 
  }
};

void AccessController::waitForACard(void *pvParameters) {
  AccessController* accessControler = (AccessController*) pvParameters;
  while(true) {
    if (accessControler->rfid.read()) {
      accessControler->checkCardValidity();
    }

    vTaskDelay(10);
  } 
};

void AccessController::init() {
  xTaskCreate(AccessController::waitForACard, "WaitForACard", 8192, this, 4, NULL);
};
