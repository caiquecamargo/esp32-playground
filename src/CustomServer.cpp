#include <WebServer.h>
#include <ArduinoJson.h>
#include "CustomServer.h"

WebServer webServer(80);

StaticJsonDocument<250> jsonDocument;
char buffer[250];

void createJson(const char *response) {
  jsonDocument.clear();
  jsonDocument["response"] = response;
  serializeJson(jsonDocument, buffer);
}

void simpleResponse() {
  Serial.println("Creating response...");

  const char *response = "Just a simple test";
  createJson(response);
  
  webServer.send(200, "application/json", buffer);
}

void createRoutes() {
  Serial.println("Creating Server routes...");
  webServer.on("/test", simpleResponse);
}

void initServer() {
  Serial.println("Initializing Server...");
  webServer.begin();
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

void CustomServer::init() {
  createAccessPoint();
  createRoutes();
  initServer();
}

void CustomServer::handleClient() {
  webServer.handleClient();
}