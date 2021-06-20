#include "Routes.h"
#include "User.h"
#include "WebServer.h"

class UserRoutes : public Routes {
  public:
    WebServer& webServer;
    UserRoutes(WebServer& webServer);
    WebServer::THandlerFunction findById();
    void create();
};