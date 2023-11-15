#ifdef ARDUINO

#include "utils.h"
#include "debugUtils.hpp"
#include "plan.hpp"

Plan::Plan(const JsonObject &jsonPlan)
{
    duration = jsonPlan["duration"];
    repeatMethod = jsonPlan["repeatMethod"];
    sscanf(jsonPlan["startTime"], "%hhu:%hhu", &startTime.hour, &startTime.minute);
    const char *lastTimeStr = jsonPlan["lastTime"] | "1.1.23";
    sscanf(lastTimeStr, "%hhu.%hhu.%hhu", &lastTime.day, &lastTime.month, &lastTime.year);

    JsonArray daysJson;
    switch (repeatMethod)
    {
    case Plan::REPEAT_METHOD_DAILY:
        repeat = jsonPlan["repeat"];
        break;
    case Plan::REPEAT_METHOD_WEEKLY:
        repeat = 0;
        daysJson = jsonPlan["repeat"];
        if (daysJson)
        {
            for (uint8 day : daysJson)
            {
                SET_BIT(repeat, day);
            }
        }
        break;
    case Plan::REPEAT_METHOD_NO_REPEAT:
    default:
        repeat = 0;
        break;
    }

    // DEBUG_MODE_PRINT_VALUES("\t\tPlan ===");
    // DEBUG_MODE_PRINT_VALUES("\t\tstartTime: [hh:mm] ", startTime.hour, ":", startTime.minute);
    // DEBUG_MODE_PRINT_VALUES("\t\tlastTime [dd.mm.yy] ", lastTime.day, ".", lastTime.month, ".20", lastTime.year);
    // DEBUG_MODE_PRINT_VALUES("\t\trepeatMethod ", repeatMethod);
    // DEBUG_MODE_PRINT_VALUES("\t\trepaet ", repeat);
    // DEBUG_MODE_PRINT_VALUES("\t\tduration ", duration);
}

void Plan::update(const JsonObject &jsonPlan)
{
    duration = jsonPlan["duration"] | duration;
    if (jsonPlan.containsKey("startTime"))
    {
        sscanf(jsonPlan["startTime"], "%hhu:%hhu", &startTime.hour, &startTime.minute);
    }
    if (jsonPlan.containsKey("lastTime") && jsonPlan["lastTime"])
    {
        sscanf(jsonPlan["lastTime"], "%hhu.%hhu.%hhu", &lastTime.day, &lastTime.month, &lastTime.year);
    }
    if (jsonPlan.containsKey("repeatMethod") && jsonPlan.containsKey("repeat"))
    {
        repeatMethod = jsonPlan["repeatMethod"];
        JsonArray daysJson;
        switch (repeatMethod)
        {
        case Plan::REPEAT_METHOD_DAILY:
            repeat = jsonPlan["repeat"];
            break;
        case Plan::REPEAT_METHOD_WEEKLY:
            repeat = 0;
            daysJson = jsonPlan["repeat"];
            if (daysJson)
            {
                for (uint8 day : daysJson)
                {
                    SET_BIT(repeat, day);
                }
            }
            break;
        case Plan::REPEAT_METHOD_NO_REPEAT:
        default:
            repeat = REPEAT_METHOD_NO_REPEAT;
            break;
        }
    }

    // DEBUG_MODE_PRINT_VALUES("\t\tPlan ===");
    // DEBUG_MODE_PRINT_VALUES("\t\tstartTime: [hh:mm] ", startTime.hour, ":", startTime.minute);
    // DEBUG_MODE_PRINT_VALUES("\t\tlastTime [dd.mm.yy] ", lastTime.day, ".", lastTime.month, ".20", lastTime.year);
    // DEBUG_MODE_PRINT_VALUES("\t\trepeatMethod ", repeatMethod);
    // DEBUG_MODE_PRINT_VALUES("\t\trepaet ", repeat);
    // DEBUG_MODE_PRINT_VALUES("\t\tduration ", duration);
}

bool Plan::start(uint8 year, uint8 month, uint8 day)
{
    lastTime.year = year;
    lastTime.month = month;
    lastTime.day = day;

    // todo update firebase
    return true;
}

void Plan::getLastTimeStr(char lastTimeBuff[LAST_TIME_MAX_LEN]) const
{
    snprintf(lastTimeBuff, LAST_TIME_MAX_LEN, "%hhu.%hhu.%hhu", lastTime.day, lastTime.month, lastTime.year);
}

#endif // ARDUINO
