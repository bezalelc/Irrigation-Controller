#ifdef ARDUINO

#include <EEPROM.h>
#include "utils.h"
#include "debugUtils.hpp"
#include "configData.hpp"

ConfigData::ConfigData()
{
}

void ConfigData::init()
{
    EEPROM.begin(sizeof(ConfigData));

    DEBUG_MODE_PRINT_NAMES_VALUES(sizeof(ConfigData));
    uint8_t *dataPtr = reinterpret_cast<uint8_t *>(this);

    for (uint16_t address = 0; address < sizeof(ConfigData); address++)
    {
        dataPtr[address] = EEPROM.read(address);
    }

#ifdef ESP32
    // todo temp solution for esp32 eeprom
    strncpy(wifiSSID, "Bezalel&Adi", sizeof("Bezalel&Adi") + 1);
    strncpy(wifiPassword, "liberman56", sizeof("liberman56") + 1);
    strncpy(apSSID, "thermometer", sizeof("thermometer") + 1);
    strncpy(apPassword, "thermometer", sizeof("thermometer") + 1);
    strncpy(userUser, "bezalelcohen1@gmail.com", sizeof("bezalelcohen1@gmail.com") + 1);
    strncpy(userPassword, "123456", sizeof("123456") + 1);
#endif

    DEBUG_MODE_PRINT_NAMES_VALUES(wifiSSID, wifiPassword, apSSID, apPassword, userUser, userPassword);
}

ConfigData &ConfigData::getInstance()
{
    static ConfigData instance;
    return instance;
}

void ConfigData::updateConfig(CONFIG_DATA_TYPE dataType, const String buff)
{
    uint8 offset = BUFF_MAX_LEN * dataType, address = 0;
    DEBUG_MODE_PRINT_NAMES_VALUES(dataType, offset, buff, buff.length());

    char *dataPtr = reinterpret_cast<char *>(this);

    for (; address < buff.length(); address++, offset++)
    {
        dataPtr[offset] = buff[address];
        EEPROM.write(offset, buff[address]);
    }
    dataPtr[offset] = 0;
    EEPROM.write(offset, 0);
    EEPROM.commit();
}

const char &ConfigData::getUsername() const
{
    return *userUser;
}

const char &ConfigData::getUserPassword() const
{
    return *userPassword;
}

#endif // ARDUINO
