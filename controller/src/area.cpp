#ifdef ARDUINO

#include "firebaseHandler.hpp"
#include "area.hpp"
#include "irrigationManager.hpp"

constexpr uint8 OPEN_TIME_MAX_LEN = 25; // todo 18

Area::Area(uint8 id, const JsonObject &jsonArea, IrrigationManager &irrigationManager, FirebaseHandler &firebaseHandler)
    : id(id), irrigationManager(irrigationManager), firebaseHandler(firebaseHandler)
{
    close = jsonArea["close"];
    isOpen = false;  // jsonArea["isOpen"];
    activePlan = -1; // jsonArea["activePlan"];
    manualOpen = jsonArea["manualOpen"];
    fertilizer = jsonArea["fertilizer"];

    sscanf(jsonArea["openTime"], "%hhu.%hhu.%hhu %hhu:%hhu:%hhu",
           &openTime.day, &openTime.month, &openTime.year,
           &openTime.hour, &openTime.minute, &openTime.secound);

    // DEBUG_MODE_PRINT_VALUES("\tArea ===");
    // DEBUG_MODE_PRINT_VALUES("\tclose: ", close);
    // DEBUG_MODE_PRINT_VALUES("\tisOpen ", isOpen);
    // DEBUG_MODE_PRINT_VALUES("\tactivePlan ", activePlan);
    // DEBUG_MODE_PRINT_VALUES("\tmanualOpen ", manualOpen);
    // DEBUG_MODE_PRINT_VALUES("\tfertilizer ", fertilizer);
    // DEBUG_MODE_PRINT_VALUES("\topenTime [dd.mm.yy hh:mm:ss] ", openTime.day, ".", openTime.month, ".20", openTime.year, " ",
    //                         openTime.hour, ":", openTime.minute, ":", openTime.secound);

    for (uint8 i = 0; i < MAX_PLANS_IN_AREA; i++)
    {
        plans[i] = nullptr;
    }
    JsonObject plansJson = jsonArea["plans"];
    if (plansJson)
    {
        for (JsonPair planPair : plansJson)
        {
            uint8 planId = ((String)(planPair.key().c_str())).substring(2).toInt();
            plans[planId] = new Plan((JsonObject)planPair.value());
            irrigationManager.addToQueue(id, planId);
        }
    }
}

Area::~Area()
{
    for (uint8 planId = 0; planId < MAX_PLANS_IN_AREA; planId++)
    {
        delete plans[planId];
        plans[planId] = nullptr;
    }
}

void Area::update(const JsonObject &jsonArea, time_t currentTime)
{
    // isOpen = jsonArea["isOpen"] | isOpen;
    // activePlan = jsonArea["activePlan"] | activePlan;
    // manualOpen = jsonArea["manualOpen"] | manualOpen;

    if (jsonArea.containsKey("manualOpen"))
    {
        if (jsonArea["manualOpen"])
        {
            openManual(currentTime);
        }
        else
        {
            closeManual();
        }
    }
    if (jsonArea["close"])
    {
        closeManual();
    }
    fertilizer = jsonArea["fertilizer"] | fertilizer;

    if (jsonArea.containsKey("openTime"))
    {
        sscanf(jsonArea["openTime"], "%hhu.%hhu.%hhu %hhu:%hhu:%hhu",
               &openTime.day, &openTime.month, &openTime.year,
               &openTime.hour, &openTime.minute, &openTime.secound);
    }

    // DEBUG_MODE_PRINT_VALUES("\tArea ===");
    // DEBUG_MODE_PRINT_VALUES("\tclose: ", close);
    // DEBUG_MODE_PRINT_VALUES("\tisOpen ", isOpen);
    // DEBUG_MODE_PRINT_VALUES("\tactivePlan ", activePlan);
    // DEBUG_MODE_PRINT_VALUES("\tmanualOpen ", manualOpen);
    // DEBUG_MODE_PRINT_VALUES("\tfertilizer ", fertilizer);
    // DEBUG_MODE_PRINT_VALUES("\topenTime [dd.mm.yy hh:mm:ss] ", openTime.day, ".", openTime.month, ".20", openTime.year, " ",
    //                         openTime.hour, ":", openTime.minute, ":", openTime.secound);

    JsonObject plansJson = jsonArea["plans"];
    if (plansJson)
    {
        for (JsonPair planPair : plansJson)
        {
            uint8 planId = ((String)(planPair.key().c_str())).toInt();
            if (planPair.value())
            {
                if (plans[planId])
                {
                    plans[planId]->update((JsonObject)planPair.value());
                }
                else
                {
                    plans[planId] = new Plan((JsonObject)planPair.value());
                    irrigationManager.addToQueue(id, planId);
                }
            }
            else
            {
                deletePlan(planId, true);
                plans[planId] = nullptr;
            }
        }
    }
}

bool Area::closeManual()
{
    if (!isOpen || !manualOpen)
    {
        return false;
    }

    manualOpen = false;
    if (activePlan == -1)
    {
        closeTap();
    }

    return true;
}

bool Area::openManual(time_t currentTime)
{
    if (!irrigationManager.freeTap())
    {
        return false;
    }
    manualOpen = true;
    if (!isOpen)
    {
        openTap(currentTime);
    }

    return true;
}

bool Area::closePlan(uint8 planId)
{
    if (planId == -1 || activePlan != planId)
    {
        return false;
    }

    activePlan = -1;

    if (isOpen)
    {
        closeTap();
    }

    return true;
}

bool Area::openPlan(time_t currentTime, uint8 planId)
{
    if (planId == -1 || close || activePlan != -1 || !irrigationManager.freeTap())
    {
        // params error
        return false;
    }

    struct tm currentTimeTm;
    gmtime_r((time_t *)&currentTime, &currentTimeTm);

    activePlan = (int8)planId;
    DEBUG_MODE_PRINT_NAMES_VALUES(planId, activePlan);
    plans[planId]->start(currentTimeTm.tm_year - 100, currentTimeTm.tm_mon + 1, currentTimeTm.tm_mday);

    if (!isOpen && irrigationManager.freeTap())
    {
        openTap(currentTime);
    }

    return true;
}

bool Area::closeNow()
{
    if (!isOpen)
    {
        return false;
    }

    close = false;
    manualOpen = false;
    activePlan = -1;
    closeTap();

    return true;
}

void Area::openTap(time_t currentTime)
{
    struct tm currentTimeTm;
    gmtime_r((time_t *)&currentTime, &currentTimeTm);

    // todo open tap
    openTime.year = currentTimeTm.tm_year - 100;
    openTime.month = currentTimeTm.tm_mon + 1;
    openTime.day = currentTimeTm.tm_mday;
    openTime.hour = currentTimeTm.tm_hour;
    openTime.minute = currentTimeTm.tm_min;
    openTime.secound = currentTimeTm.tm_sec;

    isOpen = true;

    irrigationManager.increasOpennedTaps();
    updateFirebase(false);
}

void Area::closeTap()
{
    // todo close tap
    isOpen = true;
    close = false;

    irrigationManager.decreasOpennedTaps();
    updateFirebase(false);
}

void Area::deletePlan(uint8 planId, bool deleteQueue)
{
    if (activePlan == planId)
    {
        closePlan(planId);
    }

    if (deleteQueue)
    {
        irrigationManager.deleteFromQueue(id, planId);
    }
    firebaseHandler.deletePlan(id, planId);

    delete plans[planId];
    plans[planId] = nullptr;
}

void Area::updateFirebase(bool addNew)
{
    StaticJsonDocument<AREA_JSON_MAX_SIZE> areaJson;

    if (addNew)
    {
        areaJson["fertilizer"] = fertilizer;
        areaJson["manualOpen"] = manualOpen;
    }

    areaJson["close"] = close;
    areaJson["activePlan"] = activePlan;
    areaJson["isOpen"] = isOpen;

    char timeBuffer[OPEN_TIME_MAX_LEN];
    snprintf(timeBuffer, OPEN_TIME_MAX_LEN, "%02d.%02d.%02d %02d:%02d:%02d",
             openTime.day, openTime.month, openTime.year,
             openTime.hour, openTime.minute, openTime.secound);
    // areaJson["openTime"] = timeBuffer;

    DEBUG_MODE_PRINT_NAMES_VALUES(activePlan, isOpen, close);

    char jsonBuffer[AREA_JSON_MAX_SIZE];
    serializeJson(areaJson, jsonBuffer, AREA_JSON_MAX_SIZE);

    firebaseHandler.updateArea(id, jsonBuffer);

    // for (uint8 planId = 0; planId < MAX_PLANS_IN_AREA; planId++)
    // {
    //     /* code */
    // }
}

#endif // ARDUINO