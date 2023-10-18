#if defined(ARDUINO) && !defined(EEPROM_MANAGER)
#define EEPROM_MANAGER

#include <EEPROM.h>
// #include "debugUtils.hpp"
#include "configData.hpp"

#define CONFIG_SIZE 33*6
#define SETTING_SIZE 33*6
#define AREA_SIZE 33*6
#define PLAN_SIZE 33*6

typedef enum EEPROM_LOCATION
{
    EEPROM_LOCATION_CONFIG,
} EEPROM_LOCATION;

class EepromManager
{
private:
    /* data */
public:
    EepromManager(/* args */);
    ~EepromManager();
};

EepromManager::EepromManager(/* args */)
{
}

EepromManager::~EepromManager()
{
}

#endif // defined(ARDUINO) && !defined(EEPROM_MANAGER)