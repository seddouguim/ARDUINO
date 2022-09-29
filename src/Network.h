#ifndef SRC_NETWORK
#define SRC_NETWORK

// ARDUINO INCLUDES
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// ESP INCLUDES
// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>

#include <ArduinoJson.h>

class Network
{
public:
    Network();
    void run(StaticJsonDocument<256> data);

private:
    String uuid;
    const char *ssid;
    const char *password;
    bool is_connected;
    unsigned long last_network_call;
    void postData(StaticJsonDocument<256> data);
    void connect();
    void get_name();
};

#endif /* SRC_NETWORK */
