#if defined(ARDUINO) && !defined(NETWORKN_HPP)
#define NETWORKN_HPP

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ctime>
#include "configData.hpp"

class Network
{
public:
    static bool getConfigMode();
    void APMode();
    void connectToWifi();
    time_t getNTPDate(uint8 UTC = 3);

    static Network &getInstance(ConfigData &configData);

    ~Network();

    // typedef enum CONFIG_MODE
    // {
    //     CONFIG_MODE_THERMOMETER,
    //     CONFIG_MODE_AP,

    // }CONFIG_MODE;

private:
    ESP8266WebServer server;
    ConfigData &configData;
    time_t ntpTime;

    static Network *instance;
    static bool configMode;

    Network(ConfigData &configData);
    static void IRAM_ATTR setAP();
    void handleUpdate();

    // Private copy constructor and assignment operator to prevent copying
    Network(const Network &) = delete;
    Network &operator=(const Network &) = delete;
};

#endif // defined(ARDUINO) && !defined(NETWORKN_HPP)