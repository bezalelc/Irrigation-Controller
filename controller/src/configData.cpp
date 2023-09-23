#ifdef ARDUINO

#include <EEPROM.h>
#include "debugUtils.hpp"
#include "configData.hpp"

ConfigData::ConfigData()
{
    EEPROM.begin(sizeof(ConfigData));

    DEBUG_MODE_PRINT_NAMES_VALUES(sizeof(ConfigData));
    uint8_t *dataPtr = reinterpret_cast<uint8_t *>(this);

    for (uint16_t address = 0; address < sizeof(ConfigData); address++)
    {
        dataPtr[address] = EEPROM.read(address);
    }

    DEBUG_MODE_PRINT_NAMES_VALUES(wifiSSID, wifiPassword, apSSID, apPassword, userUser, userPassword);
}

ConfigData &ConfigData::getInstance()
{
    static ConfigData instance;
    return instance;
}

void ConfigData::updateConfig(CONFIG_DATA_TYPE dataType, const String buff)
{
    uint8_t offset = BUFF_MAX_LEN * dataType, address = 0;
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
