#include "server/CustomServer.h"

AsyncWebServer webServer(80);
AsyncWebSocket webSocket("/ws");
UserSerializer userSerializer;
UserService userService;
RFID rfidServer;

struct UserMsg{
  std::string message;
  uint32_t clientId;
};

UserMsg userMsg;
const static int WEBSOCKET_REQUEST_TIMEOUT = 1000;

void sendMessage(void *arg) {
  Log::logS("WS: Client " + (std::string) String(userMsg.clientId).c_str(), "Running user create task...");
  int count = WEBSOCKET_REQUEST_TIMEOUT;
  std::string id;

  MutexController::take();
  while(count){
    vTaskDelay(10);
    count--;
    id = rfidServer.read();
    if (id.compare("") != 0) {
      break;
    }
  }
  MutexController::give();

  if (id.compare("") != 0){
    User user;
    user.name = userMsg.message;
    user.cardId = id;

    if (webSocket.hasClient(userMsg.clientId) &&
      webSocket.client(userMsg.clientId)->canSend()) {

      if (userService.create(user)) {
        std::string jsonUser = userSerializer.createJson(user);
        webSocket.text(userMsg.clientId, jsonUser.c_str());
      } else {
        std::string error = userSerializer.error("Cartão já cadastrado ou erro ao criar usuário!");
        webSocket.text(userMsg.clientId, error.c_str());
        Log::logS("WS: Client " + (std::string) String(userMsg.clientId).c_str(), "Card already registered or error when creating user!");
      }
    }
  } else {
    std::string error = userSerializer.error("Tempo de espera estourado!");
    webSocket.text(userMsg.clientId, error.c_str());
    Log::logS("WS: Client " + (std::string) String(userMsg.clientId).c_str(), "Overdue waiting time!");
  }
  
  vTaskDelete(NULL);
}

void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  const std::string REPOSITORY = "ws[" + (std::string) server->url() + 
                                 (std::string) "][" + 
                                 (std::string) String(client->id()).c_str() + 
                                 (std::string)"]";
  if(type == WS_EVT_CONNECT){
    Log::logS(REPOSITORY, "connect");
  } else if(type == WS_EVT_DISCONNECT){
    Log::logS(REPOSITORY, "disconnect");
  } else if(type == WS_EVT_ERROR){
    Log::logS(REPOSITORY, 
      "error(" + 
      (std::string) String(*((uint16_t*)arg)).c_str() + 
      (std::string) "): " +
      (std::string) (char*)data
    );
  } else if(type == WS_EVT_PONG){
    Log::logS(REPOSITORY, 
      "pong[" + 
      (std::string) String(len).c_str() + 
      (std::string) "]: " +
      (std::string) ((len)?(char*)data:"")
    );
  } else if(type == WS_EVT_DATA){
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    std::string msg = "";
    if(info->final && info->index == 0 && info->len == len){
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

      Log::logS(REPOSITORY, 
        (info->opcode == WS_TEXT)?"text":"binary" + 
        (std::string) "-message[" + 
        (std::string) String((long)info->len).c_str() + 
        (std::string) "]: " +
        msg
      );

      userMsg.message = msg;
      userMsg.clientId = client->id();

      xTaskCreate(sendMessage, "TaskSendMessage", 8192, NULL, 10, NULL);
    } else {
      Log::logS(REPOSITORY, "Request type not accespted!");
      client->text("Request type not accespted!");
    }
  }
}

CustomServer::CustomServer(const char *apSsid, const char *apPassword) : 
  Log("Server"),
  apSsid(apSsid),
  apPassword(apPassword)
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
  std::string method = request->methodToString();
  AsyncWebServerResponse *response = request->beginResponse(404, "text/plain", "Resource not found or not exists.");

  if (method.compare("OPTIONS")) response->addHeader("status", "OK");

  serverLog(request->methodToString(), request->url().c_str());
  log("Resource not found. Redirecting to /");

  request->redirect("/");
}

void CustomServer::createUserHandler(AsyncWebServerRequest *request) {

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
  rfidServer.init();
}