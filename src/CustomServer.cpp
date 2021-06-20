#include "customServer.h"

WebServer webServer(80);
UserSerializer userSerializer;

const char* www_username = "admin";
const char* www_password = "esp32";

void simpleResponse() {
  if (!webServer.authenticate(www_username, www_password)) return webServer.requestAuthentication();

  Serial.println("Creating response...");

  const char *response = "Just a simple test";
  User user;
  user.cardId = "123456789";
  user.name = "Maria das Graças";

  userSerializer.create(user);

  std::string jsonResponse = userSerializer.stringify(response);

  webServer.send(200, "application/json", jsonResponse.c_str());
}

void indexHandler() {
  Serial.println("Request on /");
  FileHandler::sendFile("/home.html", "text/html", webServer);
}

void cssCssHandler() {
  FileHandler::sendFile("/home.css", "text/css", webServer);
}

void jsHandler() {
  FileHandler::sendFile("/home.js", "text/js", webServer);
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
  webServer.on("/test", simpleResponse);

  webServer.on("/", indexHandler);
  webServer.on("/home.css", indexCssHandler);
}

void CustomServer::initServer() {
  Serial.println("Initializing Server...");
  webServer.begin();
}

void CustomServer::init() {
  createAccessPoint();
  createRoutes();
  initServer();
}

void CustomServer::handleClient() {
  webServer.handleClient();
}