#if defined(ARDUINO) && !defined(SETTING_HPP)
#define SETTING_HPP

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ctime>
#include "utils.h"
#include "debugUtils.hpp"

class Setting
{
public:
    enum UPDATE_METHOD
    {
        UPDATE_METHOD_CONTINUOUS,
        UPDATE_METHOD_EVERY_MINUTE,
        UPDATE_METHOD_EVERY_5_MINUTE,
        UPDATE_METHOD_EVERY_30_MINUTE,
        UPDATE_METHOD_EVERY_HOUR,
        UPDATE_METHOD_DAY,
        UPDATE_METHOD_MAX_VAL,
    };
    Setting() : maxTaps(2), updateMethod(UPDATE_METHOD_CONTINUOUS), UTC(3) {}
    Setting(const JsonObject &jsonSetting)
    {
        maxTaps = jsonSetting["maxTaps"];
        updateMethod = jsonSetting["updateMethod"];
        UTC = jsonSetting["UTC"];
        DEBUG_MODE_PRINT_VALUES("\tSetting ===");
        DEBUG_MODE_PRINT_VALUES("\tmaxTaps: ", maxTaps);
        DEBUG_MODE_PRINT_VALUES("\tupdateMethod: ", updateMethod);
        DEBUG_MODE_PRINT_VALUES("\tUTC: ", UTC);
    }
    void update(const JsonObject &jsonSetting)
    {
        maxTaps = jsonSetting["maxTaps"] | maxTaps;
        updateMethod = jsonSetting["updateMethod"] | updateMethod;
        UTC = jsonSetting["UTC"] | UTC;
        // DEBUG_MODE_PRINT_VALUES("\tSetting ===");
        // DEBUG_MODE_PRINT_VALUES("\tmaxTaps: ", maxTaps);
        // DEBUG_MODE_PRINT_VALUES("\tupdateMethod: ", updateMethod);
        // DEBUG_MODE_PRINT_VALUES("\tUTC: ", UTC);
    }

private:
    uint8 maxTaps;
    UPDATE_METHOD updateMethod;
    uint8 UTC; // 5bit time zone: [0,23]

    friend class IrrigationManager;
};

constexpr time_t updateMethodDelaySecounds[Setting::UPDATE_METHOD_MAX_VAL] = {
    [Setting::UPDATE_METHOD_CONTINUOUS] = 0,
    [Setting::UPDATE_METHOD_EVERY_MINUTE] = MINUTES_TO_SECOUNDS(1),
    [Setting::UPDATE_METHOD_EVERY_5_MINUTE] = MINUTES_TO_SECOUNDS(5),
    [Setting::UPDATE_METHOD_EVERY_30_MINUTE] = MINUTES_TO_SECOUNDS(30),
    [Setting::UPDATE_METHOD_EVERY_HOUR] = HOURS_TO_SECOUNDS(1),
    [Setting::UPDATE_METHOD_DAY] = DAYS_TO_SECOUNDS(1),
};

#endif // defined(ARDUINO) && !defined(SETTING_HPP)