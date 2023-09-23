#if defined(ARDUINO) && !defined(USER_DATA_HPP)
#define USER_DATA_HPP

#include <Arduino.h>
#include <ArduinoJson.h>
#include "debugUtils.hpp"

// 4096
// #include "configData.hpp"

// #define EEPROM_OFFSET (BUFF_MAX_LEN * 6)
// #define DEFAULT_UTC 2

class UserData
{
public:
    typedef struct Setting
    {
        enum UPDATE_METHOD
        {
            UPDATE_METHOD_CONTINUOUS,
            UPDATE_METHOD_EVERY_MINUTE,
            UPDATE_METHOD_EVERY_5_MINUTE,
            UPDATE_METHOD_EVERY_30_MINUTE,
            UPDATE_METHOD_EVERY_HOUR,
            UPDATE_METHOD_DAY,
            UPDATE_METHOD_MAX_VAL,
        } updateMethod;
        uint8 UTC;     // 5bit time zone: [0,23]
        uint8 maxTaps; // 5bit [1,32]
    } Setting;

    typedef struct Plan
    {
        struct LastTimeDate
        {
            uint8 day;
            uint8 month;
            uint8 year;
        } lastTime;

        struct StartTime
        {
            uint8 hour;
            uint8 minute;
        } startTime; // 11bit in minutes relative to the start day

        uint8 duration; // 0-256 minutes
        enum REPEAT_METHOD
        {
            REPEAT_METHOD_DAILY,
            REPEAT_METHOD_WEEKLY,
            REPEAT_METHOD_NO_REPEAT,
            REPEAT_METHOD_MAX_VAL
        } repeatMethod;
        uint8 repaet; // no repeat: 0 | daily [0,256] | weekly bitmap for 7 days
    } Plan;

    typedef struct Area
    {
        bool close;
        bool isOpen;
        int8 activePlan; // 3bit options: [0,7]
        bool manualOpen;
        bool fertilizer;

        struct OpenTimeDate
        {
            uint8 year;
            uint8 month;
            uint8 day;
            uint8 hour;
            uint8 minute;
            uint8 secound;
        } openTime;
        uint8 plansLen; // 3bit options: [0,7]
        Plan *plans;
    } Area;

    void update(const JsonObject &jsonUserData);
    uint8 getAreasLen() const;
    static UserData &getInstance();
    ~UserData();

private:
    Area *areas;
    uint8 areasLen; // 5bit [0,32]
    Setting setting;
    static UserData *instance;

    UserData();
    // UserData(const JsonObject &jsonUserData);

    // Private copy constructor and assignment operator to prevent copying
    UserData(const UserData &) = delete;
    UserData &operator=(const UserData &) = delete;

    void setArea(Area &area, const JsonObject &jsonArea);
    void setPlan(Plan &plan, const JsonObject &jsonPlan);
    void setSetting(const JsonObject &jsonSetting);
    void freeHeapMemory();

    friend class IrrigationManager;
};

#endif // defined(ARDUINO) && !defined(USER_DATA_HPP)