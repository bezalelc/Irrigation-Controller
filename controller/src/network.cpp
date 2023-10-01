#ifdef ARDUINO

#include "network.hpp"
#include "debugUtils.hpp"

#define MAX_CONNECTION 4
#define CONFIG_INPUT_PIN D2
#define SERVER_PORT 80
// TODO: define IP 192.168.4.1

#define UTC_OFFSET_INSECONDS(utc) (utc * 3600)

Network *Network::instance = nullptr;
bool Network::configMode = false;

Network::Network(ConfigData &configData) : server(SERVER_PORT), configData(configData), ntpTime(0)
{

    pinMode(CONFIG_INPUT_PIN, INPUT_PULLUP);
    // attachInterruptArg(digitalPinToInterrupt(CONFIG_INPUT_PIN), &Network::setAP, this, CHANGE);
    configMode = !digitalRead(CONFIG_INPUT_PIN);
    attachInterrupt(digitalPinToInterrupt(CONFIG_INPUT_PIN), &Network::setAP, CHANGE);
}

Network &Network::getInstance(ConfigData &configData)
{
    if (!instance)
    {
        instance = new Network(configData);
    }
    return *instance;
}

Network::~Network()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

bool Network::getConfigMode()
{
    return configMode;
}

void Network::handleUpdate()
{
    WiFiClient client = server.client();
    // String userMessage = server.hasArg("plain") ? server.arg("plain") : "";
    bool isValidRequest = server.args() > 0 && server.args() < 7;
    for (int i = 0; i < server.args() && isValidRequest; i++)
    {
        isValidRequest = server.argName(i).length() == 1 &&
                         server.argName(i)[0] >= '0' &&
                         server.argName(i)[0] < '7' &&
                         server.arg(i).length() > 0 &&
                         server.arg(i).length() <= BUFF_MAX_LEN;
    }

    DEBUG_MODE_PRINT_NAMES_VALUES(server.args(), isValidRequest);

    server.sendHeader("Access-Control-Allow-Origin", "*");
    if (isValidRequest)
    {
        for (int i = 0; i < server.args(); i++)
        {
            DEBUG_MODE_PRINT_NAMES_VALUES(i, server.argName(i), server.arg(i));
            configData.updateConfig((ConfigData::CONFIG_DATA_TYPE)(server.argName(i)[0] - '0'), server.arg(i));
        }
        server.send(200, "text/plain", "Updated successfully, you can close this page\n");
    }
    else
    {
        server.send(400, "text/plain", "Oops... Something went wrong!!!   ):\n");
    }
}

void Network::APMode()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(configData.apSSID, configData.apPassword, 1, false, MAX_CONNECTION);
    server.begin();
    server.on("/config", HTTP_GET, [this]()
              { this->handleUpdate(); });

    DEBUG_MODE_PRINT_NAMES_VALUES(WiFi.softAPIP(), configData.apSSID, configData.apPassword);
    while (configMode)
    {
        server.handleClient();
        delay(200);
    }

    server.stop();
    WiFi.softAPdisconnect();
    // delay 1 sec for cleanup
    delay(1000);
}

void Network::connectToWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(configData.wifiSSID, configData.wifiPassword);
    DEBUG_MODE_PRINT_VALUES("Connecting to ", configData.wifiSSID);
    DEBUG_MODE_PRINT_NAMES_VALUES(configData.wifiSSID, configData.wifiPassword);
    while (!configMode && WiFi.status() != WL_CONNECTED)
    {
        DEBUG_MODE_SERIAL_PRINT(".");
        delay(300);
    }
    DEBUG_MODE_SERIAL_PRINTLN(" Connected!");
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    DEBUG_MODE_PRINT_VALUES("IP address for network ", configData.wifiSSID, ": ", WiFi.localIP());
}

time_t Network::getNTPDate(uint8 utc)
{
    if (ntpTime)
    {
        return ntpTime + millis() / 1000;
    }

    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET_INSECONDS(utc));
    timeClient.begin();
    timeClient.update();
    ntpTime = timeClient.getEpochTime();
    timeClient.end();
    return ntpTime;
}

/**
 * interupt function: update the flage for AP configuration/WiFi thermometer mode
 */
void IRAM_ATTR Network::setAP()
{
    configMode = !(bool)digitalRead(CONFIG_INPUT_PIN);
}

#endif // ARDUINO
