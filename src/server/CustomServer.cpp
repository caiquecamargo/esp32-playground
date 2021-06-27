#include "server/CustomServer.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_ipc.h>

AsyncWebServer webServer(80);
AsyncWebSocket webSocket("/ws");
UserSerializer userSerializer;
UserService userService("/spiffs/test.db");

struct UserMsg{
  std::string message;
  uint32_t clientId;
};

UserMsg userMsg;

void sendMessage(void *arg) {
  Serial.printf("UserMsg: %s\n", userMsg.message.c_str());
  Serial.printf("ClientId: %u\n", userMsg.clientId);

  int count = 30;
  while(count){
    vTaskDelay(1000);
    count--;
    Serial.printf("Task waiting ...\n");
  }

  User user;
  user.name = userMsg.message;
  user.cardId = Utils::generateRandomId();

  AsyncWebSocketClient* client = webSocket.client(userMsg.clientId);

  if (client->canSend()) {
    if (userService.create(user)) {
      std::string jsonUser = userSerializer.createJson(user);
      Serial.println(jsonUser.c_str());

      webSocket.text(userMsg.clientId, jsonUser.c_str());
    } else {
      Serial.println("Erro ao criar usuário.");
    }
  } else {
    Serial.println("Erro ao criar usuário.");
  }
  
  vTaskDelete(NULL);
}

void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  if(type == WS_EVT_CONNECT){
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
  } else if(type == WS_EVT_DISCONNECT){
    Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
  } else if(type == WS_EVT_ERROR){
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < info->len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());

      userMsg.message = msg.c_str();
      userMsg.clientId = client->id();

      xTaskCreate(sendMessage, "TaskSendMessage", 8192, NULL, 4, NULL);
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());

      if((info->index + len) == info->len){
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}

CustomServer::CustomServer(const char *apSsid, const char *apPassword) : 
  Log("Server"),
  apSsid(apSsid),
  apPassword(apPassword)
  // webServer(AsyncWebServer(80)),
  // webSocket(AsyncWebSocket("/ws")),
  // userService(UserService("/spiffs/test.db"))
{}

void CustomServer::serverLog(std::string method, std::string uri) {
  Log::logS("Server", method + " request on: " + uri);
}

std::string CustomServer::getPathParam(std::string root, std::string url) {
  int rootSize = root.size();
  url.erase(0, rootSize + 1);

  return url;
}

int CustomServer::hasPathParam(std::string root, std::string url) {
  int rootSize = root.size();
  url.erase(0, rootSize + 1);

  return url.size();
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
  log("Resource not found. Redirecting to /");

  request->redirect("/");
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

  if (!request->hasParam("name")) {
    request->send(400, "text/plain", "User does have a name to complete the task!");
    return;
  }

  User user;
  user.name = request->getParam("name")->value().c_str();
  user.cardId = Utils::generateRandomId();

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

  std::string url = request->url().c_str();
  std::string jsonUser;

  if (request->hasParam("name")) {
    std::string name = request->getParam("name")->value().c_str();
    userService.findByName(name);
    jsonUser = userSerializer.createJson(userService.resultSet);
  } else if (hasPathParam("/user", url)) {
    std::string id = getPathParam("/user", url);
    userService.findById(id);
    jsonUser = userSerializer.createJson(userService.resultSet[0]);
  } else {
    userService.findAll();
    jsonUser = userSerializer.createJson(userService.resultSet);
  }

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

  webSocket.onEvent(onEvent);
  webServer.addHandler(&webSocket);
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