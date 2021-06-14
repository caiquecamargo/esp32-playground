#include <WiFi.h>

class CustomServer {
  public:
    CustomServer(const char *apSsid, const char *apPassword);
    void init();
    void handleClient();
  private:
    const char *apSsid;
    const char *apPassword;
    IPAddress ip;
    void createAccessPoint();
};