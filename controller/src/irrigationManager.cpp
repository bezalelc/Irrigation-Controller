#ifdef ARDUINO

#include "debugUtils.hpp"
#include "irrigationManager.hpp"

#define MAX_UINT32 (0xFFFFFFFFU)
#define IS_BIT_SET(variable, bit) ((variable & (1 << bit)) != 0)

#define MINUTES_TO_SECOUNDS(minutes) (minutes * 60)
#define HOURS_TO_SECOUNDS(hours) (hours * MINUTES_TO_SECOUNDS(60))
#define DAYS_TO_SECOUNDS(days) (days * HOURS_TO_SECOUNDS(24))
const uint32 updateMethodDelaySecounds[UserData::Setting::UPDATE_METHOD_MAX_VAL] = {
    [UserData::Setting::UPDATE_METHOD_CONTINUOUS] = 0,
    [UserData::Setting::UPDATE_METHOD_EVERY_MINUTE] = MINUTES_TO_SECOUNDS(1),
    [UserData::Setting::UPDATE_METHOD_EVERY_5_MINUTE] = MINUTES_TO_SECOUNDS(5),
    [UserData::Setting::UPDATE_METHOD_EVERY_30_MINUTE] = MINUTES_TO_SECOUNDS(30),
    [UserData::Setting::UPDATE_METHOD_EVERY_HOUR] = HOURS_TO_SECOUNDS(1),
    [UserData::Setting::UPDATE_METHOD_DAY] = DAYS_TO_SECOUNDS(1),
};

bool IrrigationManager::openArea(const PlanTime &planTime, time_t currentTime)
{

    UserData::Area &area = userData.areas[planTime.area];
    UserData::Plan &plan = area.plans[planTime.plan];

    if (opennedTaps >= userData.setting.maxTaps && !area.isOpen)
    {
        return false;
    }
    opennedTaps++;

    struct tm currentTimeTm;
    gmtime_r((time_t *)&currentTime, &currentTimeTm);

    area.activePlan = planTime.plan;
    area.isOpen = true;
    area.openTime.year = currentTimeTm.tm_year - 100;
    area.openTime.month = currentTimeTm.tm_mon + 1;
    area.openTime.day = currentTimeTm.tm_mday;
    area.openTime.hour = currentTimeTm.tm_hour;
    area.openTime.minute = currentTimeTm.tm_min;
    area.openTime.secound = currentTimeTm.tm_sec;

    plan.lastTime.year = area.openTime.year;
    plan.lastTime.month = area.openTime.month;
    plan.lastTime.day = area.openTime.day;

    // todo open tap

    return true;
}

void IrrigationManager::buildQueue()
{
    // if there was another queue -> free is memory
    delete plansQueue;

    // count plans
    uint8 plansLen = 0;
    for (uint8 i = 0; i < userData.areasLen; i++)
    {
        plansLen += userData.areas[i].plansLen;
    }

    plansQueue = new PeriorityQueue<PlanTime, time_t>(
        plansLen,
        [](PlanTime *planTime1, PlanTime *planTime2) -> bool
        { return planTime1->nextTime < planTime2->nextTime; },
        [](PlanTime *planTime, time_t time)
        { planTime->nextTime = time; });

    // insert all plans to the periority queue
    time_t nextTime, currentTime;
    struct tm currentTimeTm, lastTimeTm;
    currentTime = network->getNTPDate();
    gmtime_r((time_t *)&currentTime, &currentTimeTm);
    lastTimeTm.tm_wday = lastTimeTm.tm_yday = lastTimeTm.tm_isdst = 0;
    uint16 daysOffset;
    uint8 nextWeekDay;

    for (uint8 i = 0; i < userData.areasLen; i++)
    {
        for (uint8 j = 0; j < userData.areas[i].plansLen; j++)
        {
            // years from 2000: 2000-1900=100
            lastTimeTm.tm_year = (int)(userData.areas[i].plans[j].lastTime.year) + 100;
            lastTimeTm.tm_mon = userData.areas[i].plans[j].lastTime.month - 1;
            lastTimeTm.tm_mday = userData.areas[i].plans[j].lastTime.day;
            lastTimeTm.tm_hour = userData.areas[i].plans[j].startTime.hour;
            lastTimeTm.tm_min = userData.areas[i].plans[j].startTime.minute;
            lastTimeTm.tm_sec = 0;

            switch (userData.areas[i].plans[j].repeatMethod)
            {
            case UserData::UserData::Plan::REPEAT_METHOD_DAILY:
                daysOffset = userData.areas[i].plans[j].repaet;
                break;
            case UserData::Plan::REPEAT_METHOD_WEEKLY:
                // if (!userData.areas[i].plans[j].repaet)
                // {
                //     // error
                // }

                nextWeekDay = currentTimeTm.tm_wday;
                if (lastTimeTm.tm_year == currentTimeTm.tm_year &&
                    lastTimeTm.tm_mon == currentTimeTm.tm_mon &&
                    lastTimeTm.tm_mday == currentTimeTm.tm_mday)
                {
                    nextWeekDay = (nextWeekDay + 1) % 7;
                }
                for (daysOffset = 0; daysOffset < 7; daysOffset++, nextWeekDay = (nextWeekDay + 1) % 7)
                {
                    if (IS_BIT_SET(userData.areas[i].plans[j].repaet, nextWeekDay))
                    {
                        break;
                    }
                }
                break;
            case UserData::Plan::REPEAT_METHOD_NO_REPEAT:
                daysOffset = 0;
                break;
            default:
                // error
                daysOffset = 0;
                break;
            }

            nextTime = mktime(&lastTimeTm) + DAYS_TO_SECOUNDS((time_t)daysOffset);
            plansQueue->emplaceBack(nextTime, i, j);
        }
    }
    plansQueue->buildQueue();
}

IrrigationManager::IrrigationManager(Network *network, ConfigData &configData)
    : userData(UserData::getInstance()),
      firebaseHandler(FirebaseHandler::getInstance(configData))
{
    this->network = network;
    plansQueue = nullptr;
    opennedTaps = 0;
    defaultDelay = 0;
    nextReading = 0;
}

IrrigationManager::~IrrigationManager()
{
    delete plansQueue;
}

uint32 IrrigationManager::getNextDelay()
{
    return 0;
}

bool IrrigationManager::isOpennedTaps() const
{
    return !!opennedTaps;
}

time_t IrrigationManager::scanForNext()
{
    time_t currentTime = network->getNTPDate();
    if (nextReading < currentTime && firebaseHandler.readUserData(userData))
    {
        buildQueue();
        nextReading = currentTime + updateMethodDelaySecounds[userData.setting.updateMethod];
    }

    const PlanTime *nextPlan = plansQueue->getNext();
    if (!nextPlan)
    {
        // empty queue
        return nextReading;
    }

    if (nextPlan->nextTime <= currentTime && openArea(*nextPlan, currentTime))
    {
        plansQueue->updateNext(currentTime);
    }

    return min(plansQueue->getNext()->nextTime - currentTime, nextReading);
}

#endif // ARDUINO
