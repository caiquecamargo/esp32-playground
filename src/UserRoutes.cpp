#include "UserRoutes.h"

UserRoutes::UserRoutes(WebServer& webServer) : webServer(webServer) {}

WebServer::THandlerFunction find() {
  Serial.println("test...");
}

WebServer::THandlerFunction UserRoutes::findById() {
  Serial.println("test...");
}

void UserRoutes::create() {
  webServer.on("/user", findById);
}