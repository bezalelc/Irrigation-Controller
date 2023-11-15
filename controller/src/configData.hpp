#if defined(ARDUINO) && !defined(CONFIG_DATA_HPP)
#define CONFIG_DATA_HPP

#include <Arduino.h>

#define BUFF_MAX_LEN 33

class ConfigData
{
    friend class Network;

private:
    // static ConfigData instance;

    char wifiSSID[BUFF_MAX_LEN];
    char wifiPassword[BUFF_MAX_LEN];
    char apSSID[BUFF_MAX_LEN];
    char apPassword[BUFF_MAX_LEN];
    char userUser[BUFF_MAX_LEN];
    char userPassword[BUFF_MAX_LEN];

    // Private copy constructor and assignment operator to prevent copying
    // ConfigData(const ConfigData &) = delete;
    // ConfigData &operator=(const ConfigData &) = delete;

public:
    typedef enum CONFIG_DATA_TYPE
    {
        CONFIG_DATA_TYPE_WIFI_SSID,
        CONFIG_DATA_TYPE_WIFI_PASSWORD,
        CONFIG_DATA_TYPE_AP_SSID,
        CONFIG_DATA_TYPE_AP_PASSWORD,
        CONFIG_DATA_TYPE_USER_NAME,
        CONFIG_DATA_TYPE_USER_PASSWORD,
        CONFIG_DATA_TYPE_MAX_VAL,
    } CONFIG_DATA_TYPE;

    ConfigData();
    void init();
    static ConfigData &getInstance();
    void updateConfig(CONFIG_DATA_TYPE dataType, const String buff);
    const char &getUsername() const;
    const char &getUserPassword() const;
};

// ConfigData ConfigData::instance;

#endif // CONFIG_DATA_HPP