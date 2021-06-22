#include "server/CustomServer.h"
#include "time.h"

AsyncWebServer webServer(80);
UserSerializer userSerializer;
UserService userService = UserService("/spiffs/test.db");

const char* www_username = "admin";
const char* www_password = "esp32";

void serverLog(std::string method, std::string uri) {
  Log::logS("Server", method + " request on: " + uri);
}

void notFoundHandler(AsyncWebServerRequest *request) {
  serverLog(request->methodToString(), request->url().c_str());
  Log::logS("Server", "Resource not found");
  request->send(404, "text/plain", "Resource not found or not exists.");
}

void createUserHandler(AsyncWebServerRequest *request) {
  serverLog(request->methodToString(), request->url().c_str());

  srand(time(NULL));

  if (!request->hasParam("name")) {
    request->send(400, "text/plain", "User does have a name to complete the task!");
    return;
  }

  User user;
  user.name = request->getParam("name")->value().c_str();
  user.cardId = String(rand() % 1000 + 1000).c_str();

  if (userService.create(user)){
    std::string jsonUser = userSerializer.createJson(user);
    request->send(200, "application/Json", jsonUser.c_str());
  } else {
    request->send(500, "text/plain", "Error on create User.");
  }
}

void getUserHandler(AsyncWebServerRequest *request) {
  serverLog(request->methodToString(), request->url().c_str());

  if (request->hasParam("name")) {
    std::string name = request->getParam("name")->value().c_str();
    userService.findByName(name);
  } else if (request->hasParam("id")) {
    std::string id = request->getParam("id")->value().c_str();
    userService.findById(id);
  } else {
    userService.findAll();
  }

  std::string jsonUser = userSerializer.createJson(userService.resultSet);
  request->send(200, "application/Json", jsonUser.c_str());
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
  log("IP Address: " + (std::string) ip.toString().c_str());
}

void CustomServer::createRoutes() {
  log("Creating Server routes...");

  webServer.serveStatic("/", SPIFFS, "/index.html");
  webServer.serveStatic("/index.css", SPIFFS, "/index.css");
  webServer.serveStatic("/vendor.js", SPIFFS, "/vendor.js");
  webServer.serveStatic("/index.js", SPIFFS, "/index.js");

  webServer.on("/user", HTTP_POST, createUserHandler);
  webServer.on("/user", HTTP_GET, getUserHandler);

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