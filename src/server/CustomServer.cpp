#include "server/CustomServer.h"
#include "time.h"

WebServer webServer(80);
UserSerializer userSerializer;
UserService userService = UserService("/spiffs/test.db");

const char* www_username = "admin";
const char* www_password = "esp32";

void serverLog() {
  Serial.printf("%s request on: %s\n", webServer.method(), webServer.uri().c_str());
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

void createUserHandler() {
  serverLog();

  srand(time(NULL));

  if (!webServer.hasArg("name")) {
    webServer.send(400, "text/plain", "User does have a name to complete the task!");
    return;
  }

  User user;
  user.name = webServer.arg(0).c_str();
  user.cardId = rand() % 1000 + 1000;

  // if (userService.create(user)){
  //   std::string jsonUser = userSerializer.createJson(user);
  //   webServer.send(200, "application/Json", jsonUser.c_str());
  // } else {
  //   webServer.send(500, "text/plain", "Error on create User.");
  // }
}

CustomServer::CustomServer(const char *apSsid, const char *apPassword) {
  this->apSsid = apSsid;
  this->apPassword = apPassword;
}

void CustomServer::createAccessPoint() {
  Serial.println("Creating AccessPoint...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSsid, apPassword);
  ip = WiFi.softAPIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void CustomServer::createRoutes() {
  Serial.println("Creating Server routes...");

  webServer.on("/", indexHandler);
  webServer.on("/index.css", cssHandler);
  webServer.on("/vendor.js", indexJsHandler);
  webServer.on("/index.js", vendorJsHandler);

  webServer.on("/user", HTTP_POST, createUserHandler);
}

void CustomServer::begin() {
  webServer.begin();
}

void CustomServer::init() {
  createAccessPoint();
  createRoutes();
  begin();
}

void CustomServer::handleClient() {
  webServer.handleClient();
}