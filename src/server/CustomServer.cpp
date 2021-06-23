#include "server/CustomServer.h"
#include "time.h"

CustomServer::CustomServer(const char *apSsid, const char *apPassword) : 
  Log("Server"),
  apSsid(apSsid),
  apPassword(apPassword),
  webServer(AsyncWebServer(80)),
  userService(UserService("/spiffs/test.db"))
{}

void CustomServer::serverLog(std::string method, std::string uri) {
  Log::logS("Server", method + " request on: " + uri);
}

std::string CustomServer::getPathParam(std::string root, std::string url) {
  int rootSize = root.size();
  url.erase(0, rootSize + 1);

  return url;
}

void CustomServer::optionsHandler(AsyncWebServerRequest *request) {
  std::string method = request->methodToString();
  AsyncWebServerResponse *response = request->beginResponse(200);

  serverLog(request->methodToString(), request->url().c_str());
  log("Responding OPTIONS");

  request->send(response);
}

void CustomServer::notFoundHandler(AsyncWebServerRequest *request) {
  if (redirectHandler(request)) return;

  std::string method = request->methodToString();
  AsyncWebServerResponse *response = request->beginResponse(404, "text/plain", "Resource not found or not exists.");

  if (method.compare("OPTIONS")) response->addHeader("status", "OK");

  serverLog(request->methodToString(), request->url().c_str());
  log("Resource not found.");

  request->send(response);
}

int CustomServer::redirectHandler(AsyncWebServerRequest *request) {
  if (request->url().compareTo("/user") == 0) {
    std::string method = request->methodToString();

    if ( method.compare("PUT") == 0 ) {
      log("Redirecting from " + method + " method...");
      updateUserHandler(request);
      return 1;
    }

    if ( method.compare("DELETE") == 0 ) {
      log("Redirecting from " + method + " method...");
      deleteUserHandler(request);
      return 1;
    }
  }
  
  return 0;
}

void CustomServer::createUserHandler(AsyncWebServerRequest *request) {
  if (redirectHandler(request)) return;

  serverLog(request->methodToString(), request->url().c_str());
  log("Performing CREATE USER handler...");

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

void CustomServer::getUserHandler(AsyncWebServerRequest *request) {
  serverLog(request->methodToString(), request->url().c_str());
  log("Performing GET USER handler...");

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

void CustomServer::updateUserHandler(AsyncWebServerRequest *request) {
  serverLog(request->methodToString(), request->url().c_str());
  log("Performing UPDATE USER handler...");

  if (!request->hasParam("name")) {
    request->send(400, "text/plain", "User does have a name to complete the task!");
    return;
  }

  std::string url = request->url().c_str();

  User user;
  user.name = request->getParam("name")->value().c_str();
  user.cardId = getPathParam("/user", url);

  if (userService.update(user)){
    std::string jsonUser = userSerializer.createJson(user);
    request->send(200, "application/Json", jsonUser.c_str());
  } else {
    request->send(500, "text/plain", "Error on update User or User not exists.");
  }
}

void CustomServer::deleteUserHandler(AsyncWebServerRequest *request) {
  serverLog(request->methodToString(), request->url().c_str());
  log("Performing DELETE USER handler...");

  std::string url = request->url().c_str();
  std::string id = getPathParam("/user", url);

  if (userService.deleteItem(id)){
    request->send(200, "text/plain", id.c_str());
  } else {
    request->send(500, "text/plain", "Error on delete User or User not exists.");
  }
}

void CustomServer::createAccessPoint() {
  log("Creating AccessPoint...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSsid, apPassword);
  ip = WiFi.softAPIP();
  log("IP Address: " + (std::string) ip.toString().c_str());
}

void CustomServer::serveStatic() {
  log("Serving static files...");

  webServer.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
}

void CustomServer::createRoutes() {
  log("Creating Server routes...");

  webServer.on("/user", HTTP_GET, std::bind(&CustomServer::getUserHandler, this, std::placeholders::_1));
  webServer.on("/user", HTTP_POST, std::bind(&CustomServer::createUserHandler, this, std::placeholders::_1));
  webServer.on("/user", HTTP_PUT, std::bind(&CustomServer::updateUserHandler, this, std::placeholders::_1));
  webServer.on("/user", HTTP_DELETE, std::bind(&CustomServer::deleteUserHandler, this, std::placeholders::_1));
  webServer.on("/*", HTTP_OPTIONS, std::bind(&CustomServer::optionsHandler, this, std::placeholders::_1));

  webServer.onNotFound(std::bind(&CustomServer::notFoundHandler, this, std::placeholders::_1));
}

void CustomServer::begin() {
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");

  webServer.begin();
}

void CustomServer::init() {
  createAccessPoint();
  serveStatic();
  createRoutes();
  begin();
  userService.init();
}