#include <WiFi.h>
#include <WebServer.h>
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
    UserSerializer userSerializer;
    UserService userService;
    void createAccessPoint();
    void createRoutes();
    void begin();
    void serverLog(std::string method, std::string uri);
    void notFoundHandler(AsyncWebServerRequest *request);
    void createUserHandler(AsyncWebServerRequest *request);
    void getUserHandler(AsyncWebServerRequest *request);
};