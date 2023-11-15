#ifdef ARDUINO

#include <ArduinoJson.h>
#include "debugUtils.hpp"
#include "utils.h"
#include "irrigationManager.hpp"
#include "tapAdaptaer.hpp"

#define OPEN_TIME_MAX_LEN 25 // todo 18
#define LAST_TIME_MAX_LEN 25 // todo 9
#define AREA_JSON_MAX_SIZE 512
#define PLAN_JSON_MAX_SIZE 64

IrrigationManager::IrrigationManager(Network *network, FirebaseHandler &firebaseHandler)
    : firebaseHandler(firebaseHandler), network(network), plansQueue(nullptr), opennedTaps(0), nextReading(0), firebaseInit(false)
{
    for (uint8 areaId = 0; areaId < MAX_AREAS; areaId++)
    {
        areas[areaId] = nullptr;
    }
}

IrrigationManager::~IrrigationManager()
{
    for (uint8 areaId = 0; areaId < MAX_AREAS; areaId++)
    {
        delete areas[areaId];
    }
    delete plansQueue;
}

bool IrrigationManager::getFromFirebase()
{
    DynamicJsonDocument doc(JSON_MAX_SIZE);
    firebaseHandler.readUserData(doc, firebaseInit);

    JsonObject jsonData = doc.as<JsonObject>();
    if (jsonData["setting"])
    {
        setting.update(jsonData["setting"]);
    }

    JsonObject jsonAreas = jsonData["areas"];
    if (jsonAreas)
    {
        for (JsonPair jsonArea : jsonAreas)
        {
            uint8 areaId = ((String)(jsonArea.key().c_str())).substring(2).toInt();
            const JsonObject jsonObjectArea = (const JsonObject)jsonArea.value();
            if (jsonObjectArea)
            {
                if (areas[areaId])
                {
                    areas[areaId]->update(jsonObjectArea, network->getNTPDate(setting.UTC));
                }
                else
                {
                    areas[areaId] = new Area(areaId, jsonObjectArea, *this, firebaseHandler);
                }
            }
            else
            {
                delete areas[areaId];
                areas[areaId] = nullptr;
            }
        }
    }

    doc.clear();
    if (!firebaseInit)
    {
        buildQueue();

        firebaseInit = true;
    }

    return true;
}

void IrrigationManager::buildQueue()
{
    // if there was another queue -> free is memory
    delete plansQueue;

    plansQueue = new PeriorityQueue<PlanTime, PlanTime::Update &, MAX_TOTAL_PLANS>(
        [](PlanTime *planTime1, PlanTime *planTime2) -> bool
        { return planTime1->nextTime < planTime2->nextTime; },
        [](PlanTime *planTime, PlanTime::Update &update)
        { planTime->nextTime = update.nextTime; planTime->isOpen = update.isOpen; });

    // insert all plans to the periority queue
    time_t currentTime;
    currentTime = network->getNTPDate();

    for (uint8 areaId = 0; areaId < MAX_AREAS; areaId++)
    {
        if (areas[areaId])
        {
            for (uint8 planId = 0; planId < MAX_PLANS_IN_AREA; planId++)
            {
                if (areas[areaId]->plans[planId])
                {
                    plansQueue->emplaceBack(calculateNextPlanTime(*areas[areaId]->plans[planId], currentTime), areaId, planId, false);
                }
            }
        }
    }
    plansQueue->buildQueue();
}

void IrrigationManager::addToQueue(uint8 areaId, uint8 planId)
{
    if (plansQueue)
    {
        plansQueue->emplaceBackQueue(calculateNextPlanTime(*areas[areaId]->plans[planId], network->getNTPDate(setting.UTC)), areaId, planId, false);
    }
}

void IrrigationManager::deleteFromQueue(uint8 areaId, uint8 planId)
{
    if (plansQueue)
    {
        PlanTime::Update update = {.nextTime = 0};
        PlanTime *planTime = plansQueue->deleteQueue([areaId, planId](PlanTime *plan) -> bool
                                                     { return plan->areaId == areaId && plan->planId == planId; },
                                                     update);
        if (planTime->isOpen)
        {
            areas[areaId]->closePlan(planId);
        }
    }
}

void IrrigationManager::increasOpennedTaps()
{
    opennedTaps++;
}

void IrrigationManager::decreasOpennedTaps()
{
    opennedTaps--;
}

bool IrrigationManager::freeTap() const
{
    return opennedTaps < setting.maxTaps;
}

bool IrrigationManager::isOpennedTaps() const
{
    return !!opennedTaps;
}

time_t IrrigationManager::calculateNextPlanTime(Plan &plan, time_t currentTime)
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
    case Plan::REPEAT_METHOD_DAILY:
        daysOffset = plan.repeat;
        break;
    case Plan::REPEAT_METHOD_WEEKLY:
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
            if (IS_BIT_SET(plan.repeat, nextWeekDay))
            {
                break;
            }
        }
        break;
    case Plan::REPEAT_METHOD_NO_REPEAT:
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

time_t IrrigationManager::scanForNext()
{
    time_t currentTime = network->getNTPDate(setting.UTC);
    if (nextReading < currentTime && getFromFirebase())
    {
        nextReading = currentTime + updateMethodDelaySecounds[setting.updateMethod];
    }

    const PlanTime *nextPlan = plansQueue->getNext();
    if (!nextPlan)
    {
        // empty queue
        return nextReading;
    }

    currentTime = network->getNTPDate(setting.UTC);
    while (nextPlan->nextTime <= currentTime)
    {
        PlanTime::Update update;
        Area &area = *areas[nextPlan->areaId];
        Plan &plan = *areas[nextPlan->areaId]->plans[nextPlan->planId];

        if (nextPlan->isOpen)
        {
            area.closePlan(nextPlan->planId);
            // one time plan
            if (plan.repeatMethod == Plan::REPEAT_METHOD_NO_REPEAT)
            {
                plansQueue->removeNext();
                area.deletePlan(nextPlan->planId, false);
            }
            else
            {
                update.isOpen = false;
                update.nextTime = calculateNextPlanTime(plan, currentTime);
                plansQueue->updateNext(update);
            }
        }
        else
        {
            area.openPlan(currentTime, nextPlan->planId);
            update.isOpen = true;
            update.nextTime = currentTime + MINUTES_TO_SECOUNDS(plan.duration);
            plansQueue->updateNext(update);
        }
    }

// #if DEBUG_MODE
//     struct tm delaySecTm;
//     const char *wday[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//     for (int i = 0; i < plansQueue->getHeapLen(); i++)
//     {
//         const PlanTime *planTime = plansQueue->getHeap()[i];
//         gmtime_r((time_t *)&planTime->nextTime, &delaySecTm);
//         DEBUG_MODE_PRINT_VALUES("i=", i, ", area=", planTime->areaId, ", plan=", planTime->planId, ", isOpen=", planTime->isOpen);
//         DEBUG_MODE_SERIAL_PRINTF("[d.m.y h:m:s]=[%d.%d.%d %d:%d:%d] week day=%s\n", delaySecTm.tm_mday, delaySecTm.tm_mon + 1, delaySecTm.tm_year + 1900, delaySecTm.tm_hour, delaySecTm.tm_min, delaySecTm.tm_sec, wday[delaySecTm.tm_wday]);
//     }
// #endif

    return max(min(plansQueue->getNext()->nextTime, nextReading) - network->getNTPDate(setting.UTC), (time_t)0);
}

#endif // ARDUINO
