#if defined(ARDUINO) && !defined(PLAN_HPP)
#define PLAN_HPP

#include <Arduino.h>
#include <ArduinoJson.h>

constexpr uint8 LAST_TIME_MAX_LEN = 9;

class Plan
{
public:
    struct LastTimeDate
    {
        uint8 day;
        uint8 month;
        uint8 year;
    };

    struct StartTime
    {
        uint8 hour;
        uint8 minute;
    };

    enum REPEAT_METHOD
    {
        REPEAT_METHOD_DAILY,
        REPEAT_METHOD_WEEKLY,
        REPEAT_METHOD_NO_REPEAT,
        REPEAT_METHOD_MAX_VAL
    };

    friend class Area;
    friend class UserData;
    friend class IrrigationManager;

    Plan(const JsonObject &jsonPlan);

    void update(const JsonObject &jsonPlan);

    bool start(uint8 year, uint8 month, uint8 day);

    bool stop()
    {
        return true;
    }

    const LastTimeDate &getLastTime() const
    {
        return lastTime;
    }
    const StartTime &getStartTime() const
    {
        return startTime;
    }
    const REPEAT_METHOD getRepeatMethod() const
    {
        return repeatMethod;
    }
    const uint8 getRepeat() const
    {
        return repeat;
    }
    const uint8 getDuraion() const
    {
        return duration;
    }
    void getLastTimeStr(char lastTimeBuff[LAST_TIME_MAX_LEN]) const;

private:
    LastTimeDate lastTime;
    StartTime startTime;
    REPEAT_METHOD repeatMethod;
    uint8 repeat; // no repeat: 0 | daily [0,256] | weekly bitmap for 7 days
    uint8 duration;
    // uint8 areaId;
    // uint8 planId;
};

#endif // defined(ARDUINO) && !defined(PLAN_HPP)