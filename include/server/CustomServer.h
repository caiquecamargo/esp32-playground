#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "json/UserSerializer.h"
#include "repository/UserService.h"
#include "log/Log.h"

class CustomServer : public Log {
  public:
    CustomServer(const char *apSsid, const char *apPassword);
    void init();
  private:
    const char *apSsid;
    const char *apPassword;
    IPAddress ip;
    AsyncWebServer webServer;
    AsyncWebSocket webSocket;
    UserSerializer userSerializer;
    UserService userService;
    void createAccessPoint();
    void serveStatic();
    void createRoutes();
    void begin();
    void serverLog(std::string method, std::string uri);
    void notFoundHandler(AsyncWebServerRequest *request);
    void createUserHandler(AsyncWebServerRequest *request);
    void getUserHandler(AsyncWebServerRequest *request);
    void updateUserHandler(AsyncWebServerRequest *request);
    void deleteUserHandler(AsyncWebServerRequest *request);
    void optionsHandler(AsyncWebServerRequest *request);
    int redirectHandler(AsyncWebServerRequest *request);
    std::string getPathParam(std::string root, std::string url);
    int hasPathParam(std::string root, std::string url);
};