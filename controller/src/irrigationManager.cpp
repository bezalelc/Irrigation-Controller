#ifdef ARDUINO

#include "debugUtils.hpp"
#include "irrigationManager.hpp"

#define SECOUNDS_IN_DAY (86400U)
#define MAX_UINT32 (0xFFFFFFFFU)
#define IS_BIT_SET(variable, bit) ((variable & (1 << bit)) != 0)

#define MINUTES_TO_MILLIS(minutes) (minutes * 60 * 1000)
#define HOUR_TO_MILLIS MINUTES_TO_MILLIS(60)
#define DAY_TO_MILLIS (24 * HOUR_TO_MILLIS)
const uint32 updateMethodDelayMillis[UserData::Setting::UPDATE_METHOD_MAX_VAL] = {
    [UserData::Setting::UPDATE_METHOD_CONTINUOUS] = 0,
    [UserData::Setting::UPDATE_METHOD_EVERY_MINUTE] = MINUTES_TO_MILLIS(1),
    [UserData::Setting::UPDATE_METHOD_EVERY_5_MINUTE] = MINUTES_TO_MILLIS(5),
    [UserData::Setting::UPDATE_METHOD_EVERY_30_MINUTE] = MINUTES_TO_MILLIS(30),
    [UserData::Setting::UPDATE_METHOD_EVERY_HOUR] = HOUR_TO_MILLIS,
    [UserData::Setting::UPDATE_METHOD_DAY] = DAY_TO_MILLIS,

};

void IrrigationManager::openTap(uint8 tapIdx) {}

uint32 IrrigationManager::timeNotPass(const struct tm &currentTime, const UserData::Plan::StartTime &startTime)
{
    if (currentTime.tm_hour > startTime.hour ||
        (currentTime.tm_hour == startTime.hour && currentTime.tm_min > startTime.minute))
    {
        return 0;
    }
    else
    {
        // Calculate the number of seconds until the startTime struct.
        uint32 secondsUntil = (startTime.hour - currentTime.tm_hour) * 3600 +
                              (startTime.minute - currentTime.tm_min) * 60;
        uint32 millisecondsUntil = secondsUntil * 1000;
        return millisecondsUntil;
    }
}

bool IrrigationManager::openArea(UserData::Area &area, uint planIdx, const struct tm &currentTime)
{
    area.activePlan = planIdx;
    area.isOpen = true;
    area.openTime.year = currentTime.tm_year - 100;
    area.openTime.month = currentTime.tm_mon + 1;
    area.openTime.day = currentTime.tm_mday;
    area.openTime.hour = currentTime.tm_hour;
    area.openTime.minute = currentTime.tm_min;
    area.openTime.secound = currentTime.tm_sec;

    area.plans[planIdx].lastTime.year = area.openTime.year;
    area.plans[planIdx].lastTime.month = area.openTime.month;
    area.plans[planIdx].lastTime.day = area.openTime.day;

    // todo open tap

    return true;
}

IrrigationManager::IrrigationManager(Network *network, ConfigData &configData)
    : userData(UserData::getInstance()),
      firebaseHandler(FirebaseHandler::getInstance(configData))
{
    this->network = network;
    opennedTaps = 0;
    defaultDelay = 0;
}

IrrigationManager::~IrrigationManager() {}

uint32 IrrigationManager::getNextDelay()
{
    firebaseHandler.readUserData(userData);

    struct tm currentDay, currentTime, lastTime;
    network->getNTPDate(currentTime);
    currentDay = currentTime;
    currentDay.tm_hour = currentDay.tm_min = currentDay.tm_sec = 0;
    lastTime.tm_hour = lastTime.tm_min = lastTime.tm_sec = lastTime.tm_wday = lastTime.tm_yday = lastTime.tm_isdst = 0;

    uint8 nextArea = -1, nextPlan = -1;
    uint32 minNextTimeMillis = updateMethodDelayMillis[userData.setting.updateMethod], currentPlanTimeMillis, openTimeMillis;
    time_t currentTimestamp, lastTimestamp, difference;
    int daysBetweenDates = 0;
    for (uint8 i = 0; i < userData.areasLen; i++)
    {
        if (userData.areas[i].isOpen)
        {
            continue;
        }

        for (uint8 j = 0; j < userData.areas[i].plansLen; j++)
        {
            // years from 2000: 2000-1900=100
            lastTime.tm_year = (int)(userData.areas[i].plans[j].lastTime.year) + 100;
            lastTime.tm_mon = userData.areas[i].plans[j].lastTime.month - 1;
            lastTime.tm_mday = userData.areas[i].plans[j].lastTime.day;

            currentTimestamp = mktime(&currentDay);
            lastTimestamp = mktime(&lastTime);
            difference = difftime(currentTimestamp, lastTimestamp);
            daysBetweenDates = difference / SECOUNDS_IN_DAY;

            // if the last time was today or duration is 0
            if (daysBetweenDates == 0 || !userData.areas[i].plans[j].duration)
            {
                continue;
            }

            openTimeMillis = timeNotPass(currentTime, userData.areas[i].plans[j].startTime);
            currentPlanTimeMillis = MAX_UINT32;

            switch (userData.areas[i].plans[j].repeatMethod)
            {
            case UserData::Plan::REPEAT_METHOD_DAILY:
                currentPlanTimeMillis = openTimeMillis + DAY_TO_MILLIS * min(userData.areas[i].plans[j].repaet - daysBetweenDates, 0);
                break;

            case UserData::Plan::REPEAT_METHOD_WEEKLY:
                if (IS_BIT_SET(userData.areas[i].plans[j].repaet, currentTime.tm_wday))
                {
                    if (currentPlanTimeMillis == 0)
                    {
                        currentPlanTimeMillis = openTimeMillis;
                    }
                }
                // next day
                else if (IS_BIT_SET(userData.areas[i].plans[j].repaet, (currentTime.tm_wday + 1) % 7))
                {
                    currentPlanTimeMillis = openTimeMillis + DAY_TO_MILLIS;
                }
                break;

            case UserData::Plan::REPEAT_METHOD_NO_REPEAT:
                currentPlanTimeMillis = openTimeMillis;
                // todo: delete plan from firebase
                break;

            default:
                break;
            }

            if (openTimeMillis < MINUTES_TO_MILLIS(1))
            {
                // open now
                openArea(userData.areas[i], j, currentTime);
            }
            if (minNextTimeMillis > currentPlanTimeMillis)
            {
                minNextTimeMillis = currentPlanTimeMillis;
                nextArea = i, nextPlan = j;
            }

            DEBUG_MODE_PRINT_VALUES(i, j, daysBetweenDates);
        }
    }

    return minNextTimeMillis;
}

#endif // ARDUINO
