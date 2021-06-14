#include "CustomServer.h"
#include "SPIFFS.h"

WebServer webServer(80);
JSON json;

const char* www_username = "admin";
const char* www_password = "esp32";

void simpleResponse() {
  if (!webServer.authenticate(www_username, www_password)) return webServer.requestAuthentication();

  Serial.println("Creating response...");

  const char *response = "Just a simple test";
  json.stringfy(response);
  
  webServer.send(200, "application/json", json.buffer);
}

void fileHandler(const char* fileName, const char* fileType) {
  File file = SPIFFS.open(fileName);
  if (!file) {
    Serial.println("Failed to open file for read");
    return;
  }

  size_t fileSize = file.size();
  char buffer[fileSize + 1];
  file.read((uint8_t *)buffer, fileSize);
  buffer[fileSize] = '\0';

  file.close();

  webServer.send(200, fileType, buffer);
}

void homeHandler() {
  fileHandler("/home.html", "text/html");
}

void homeCssHandler() {
  fileHandler("/home.css", "text/css");
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
  if(!SPIFFS.begin()) {
    Serial.println("An error has ocurred while mounting SPIFFS");
    return;
  }
  
  Serial.println("Creating Server routes...");
  webServer.on("/test", simpleResponse);

  webServer.on("/", homeHandler);
  webServer.on("/home.css", homeCssHandler);
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