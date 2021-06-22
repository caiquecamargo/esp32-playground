#include "server/CustomServer.h"
#include "time.h"

WebServer webServer(80);
UserSerializer userSerializer;
UserService userService = UserService("/spiffs/test.db");

const char* www_username = "admin";
const char* www_password = "esp32";

void serverLog() {
  HTTPMethod method = webServer.method();
  Log::logS("Server", (std::string) http_method_str(method) + " request on: " + webServer.uri().c_str());
}

void indexHandler() {
  serverLog();
  FileHandler::sendFile("/index.html", "text/html", webServer);
}

void cssHandler() {
  serverLog();
  FileHandler::sendFile("/index.css", "text/css", webServer);
}

void indexJsHandler() {
  serverLog();
  FileHandler::sendFile("/index.js", "text/js", webServer);
}

void vendorJsHandler() {
  serverLog();
  FileHandler::sendFile("/vendor.js", "text/js", webServer);
}

void notFoundHandler() {
  serverLog();
  Log::logS("Server", "Resource not found");
  webServer.send(404, "text/plain", "Resource not found or not exists.");
}

void createUserHandler() {
  serverLog();

  srand(time(NULL));

  if (!webServer.hasArg("name")) {
    webServer.send(400, "text/plain", "User does have a name to complete the task!");
    return;
  }

  User user;
  user.name = webServer.arg(0).c_str();
  user.cardId = String(rand() % 1000 + 1000).c_str();

  if (userService.create(user)){
    std::string jsonUser = userSerializer.createJson(user);
    webServer.send(200, "application/Json", jsonUser.c_str());
  } else {
    webServer.send(500, "text/plain", "Error on create User.");
  }
}

CustomServer::CustomServer(const char *apSsid, const char *apPassword) : Log("Server") {
  this->apSsid = apSsid;
  this->apPassword = apPassword;
}

void CustomServer::createAccessPoint() {
  log("Creating AccessPoint...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSsid, apPassword);
  ip = WiFi.softAPIP();
  // log("IP Address: " + ip);
}

void CustomServer::createRoutes() {
  log("Creating Server routes...");

  webServer.on("/", indexHandler);
  webServer.on("/index.css", cssHandler);
  // webServer.on("/vendor.js", indexJsHandler);
  // webServer.on("/index.js", vendorJsHandler);

  webServer.on("/user", HTTP_POST, createUserHandler);
  webServer.on("/user", HTTP_GET, createUserHandler);

  webServer.onNotFound(notFoundHandler);
}

void CustomServer::begin() {
  webServer.begin();
}

void CustomServer::init() {
  createAccessPoint();
  createRoutes();
  begin();
  userService.init();
}

void CustomServer::handleClient() {
  webServer.handleClient();
}