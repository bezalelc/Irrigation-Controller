#ifdef ARDUINO

#include <ArduinoJson.h>
#include "debugUtils.hpp"
#include "irrigationManager.hpp"
#include "tapAdaptaer.hpp"

#define OPEN_TIME_MAX_LEN 25 // todo 18
#define LAST_TIME_MAX_LEN 25 // todo 9
#define AREA_JSON_MAX_SIZE 512
#define PLAN_JSON_MAX_SIZE 64

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

void IrrigationManager::init()
{

    for (uint8 areaId = 0; areaId < userData.areasLen; areaId++)
    {
        if (userData.areas[areaId].close)
        {
            userData.areas[areaId].manualOpen = false;
            userData.areas[areaId].close = false;
            userData.areas[areaId].activePlan = -1;
            closeArea(areaId);
        }
        if (userData.areas[areaId].manualOpen)
        {
            struct tm currentTimeTm;
            time_t currentTime = network->getNTPDate();
            gmtime_r(&currentTime, &currentTimeTm);
            openArea(areaId, currentTimeTm, -1);
        }
    }
}

bool IrrigationManager::openArea(uint8 areaId, const struct tm &currentTimeTm, uint8 activePlan)
{
    UserData::Area &area = userData.areas[areaId];

    if ((opennedTaps >= userData.setting.maxTaps && !area.isOpen) || area.close)
    {
        return false;
    }

    if (!area.isOpen)
    {
        TapAdapter::open(areaId);

        area.isOpen = true;
        opennedTaps++;
        area.activePlan = activePlan;

        area.openTime.year = currentTimeTm.tm_year - 100;
        area.openTime.month = currentTimeTm.tm_mon + 1;
        area.openTime.day = currentTimeTm.tm_mday;
        area.openTime.hour = currentTimeTm.tm_hour;
        area.openTime.minute = currentTimeTm.tm_min;
        area.openTime.secound = currentTimeTm.tm_sec;

        firebaseHandler.updateOpenArea(area, areaId);
    }

    return area.isOpen;
}

bool IrrigationManager::closeArea(uint8 areaId)
{
    UserData::Area &area = userData.areas[areaId];

    if (area.isOpen && !area.manualOpen)
    {
        TapAdapter::close(areaId);
        opennedTaps--;
        area.isOpen = false;
    }

    firebaseHandler.updateCloseArea(areaId);

    return !area.isOpen;
}

bool IrrigationManager::startPlan(const PlanTime &planTime, time_t currentTime)
{
    UserData::Area &area = userData.areas[planTime.area];
    UserData::Plan &plan = area.plans[planTime.plan];
    struct tm currentTimeTm;
    gmtime_r((time_t *)&currentTime, &currentTimeTm);

    if (opennedTaps >= userData.setting.maxTaps || !openArea(planTime.area, currentTimeTm, planTime.plan))
    {
        return false;
    }

    plan.lastTime.year = area.openTime.year;
    plan.lastTime.month = area.openTime.month;
    plan.lastTime.day = area.openTime.day;

    firebaseHandler.updatePlanStartTime(plan, planTime.area, planTime.plan);
    return true;
}

bool IrrigationManager::stopPlan(const PlanTime &planTime)
{
    UserData::Area &area = userData.areas[planTime.area];
    area.activePlan = -1;
    if (!area.manualOpen)
    {
        closeArea(planTime.area);
    }
    return true;
}

void IrrigationManager::buildQueue()
{
    // if there was another queue -> free is memory
    delete plansQueue;

    init();

    // count plans
    uint8 plansLen = 0;
    for (uint8 i = 0; i < userData.areasLen; i++)
    {
        plansLen += userData.areas[i].plansLen;
    }

    plansQueue = new PeriorityQueue<PlanTime, PlanTime::Update &>(
        plansLen,
        [](PlanTime *planTime1, PlanTime *planTime2) -> bool
        { return planTime1->nextTime < planTime2->nextTime; },
        [](PlanTime *planTime, PlanTime::Update &update)
        { planTime->nextTime = update.nextTime; planTime->isOpen = update.isOpen; });

    // insert all plans to the periority queue
    time_t currentTime;
    currentTime = network->getNTPDate();

    for (uint8 i = 0; i < userData.areasLen; i++)
    {
        for (uint8 j = 0; j < userData.areas[i].plansLen; j++)
        {
            plansQueue->emplaceBack(calculateNextPlanTime(userData.areas[i].plans[j], currentTime), i, j, false);
        }
    }
    plansQueue->buildQueue();
}

time_t IrrigationManager::calculateNextPlanTime(UserData::Plan &plan, time_t currentTime)
{
    time_t nextTime;
    struct tm currentTimeTm, lastTimeTm;
    gmtime_r((time_t *)&currentTime, &currentTimeTm);
    lastTimeTm.tm_wday = lastTimeTm.tm_yday = lastTimeTm.tm_isdst = 0;
    uint16 daysOffset;
    uint8 nextWeekDay;

    lastTimeTm.tm_year = (int)(plan.lastTime.year) + 100;
    lastTimeTm.tm_mon = plan.lastTime.month - 1;
    lastTimeTm.tm_mday = plan.lastTime.day;
    lastTimeTm.tm_hour = plan.startTime.hour;
    lastTimeTm.tm_min = plan.startTime.minute;
    lastTimeTm.tm_sec = 0;

    switch (plan.repeatMethod)
    {
    case UserData::UserData::Plan::REPEAT_METHOD_DAILY:
        daysOffset = plan.repaet;
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
            if (IS_BIT_SET(plan.repaet, nextWeekDay))
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
    return nextTime;
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

    while (nextPlan->nextTime <= currentTime)
    {
        PlanTime::Update update;

        if (nextPlan->isOpen)
        {
            stopPlan(*nextPlan);
            // one time plan
            if (userData.areas[nextPlan->area].plans[nextPlan->plan].repeatMethod == UserData::Plan::REPEAT_METHOD_NO_REPEAT)
            {
                plansQueue->removeNext();
            }
            else
            {
                update.isOpen = false;
                update.nextTime = calculateNextPlanTime(userData.areas[nextPlan->area].plans[nextPlan->plan], currentTime);
                plansQueue->updateNext(update);
            }
        }
        else
        {
            startPlan(*nextPlan, currentTime);
            update.isOpen = true;
            update.nextTime = currentTime + MINUTES_TO_SECOUNDS(userData.areas[nextPlan->area].plans[nextPlan->plan].duration);
            plansQueue->updateNext(update);
        }
    }

#if DEBUG_MODE
    struct tm delaySecTm;
    const char *wday[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    for (int i = 0; i < plansQueue->getHeapLen(); i++)
    {
        const PlanTime *planTime = plansQueue->getHeap()[i];
        gmtime_r((time_t *)&planTime->nextTime, &delaySecTm);
        DEBUG_MODE_PRINT_VALUES("i=", i, ", area=", planTime->area, ", plan=", planTime->plan, ", isOpen=", planTime->isOpen);
        DEBUG_MODE_SERIAL_PRINTF("[d.m.y h:m:s]=[%d.%d.%d %d:%d:%d] week day=%s\n", delaySecTm.tm_mday, delaySecTm.tm_mon + 1, delaySecTm.tm_year + 1900, delaySecTm.tm_hour, delaySecTm.tm_min, delaySecTm.tm_sec, wday[delaySecTm.tm_wday]);
    }
#endif

    return min(plansQueue->getNext()->nextTime, nextReading) - currentTime;
}

#endif // ARDUINO
