#ifdef ARDUINO

#include "userData.hpp"

#define SET_BIT(variable, bit) (variable |= (1 << bit))
// 4096

UserData *UserData::instance = nullptr;

void UserData::setArea(Area &area, const JsonObject &jsonArea)
{
    area.close = jsonArea["close"];
    area.isOpen = jsonArea["isOpen"];
    area.activePlan = jsonArea["activePlan"];
    area.manualOpen = jsonArea["manualOpen"];
    area.fertilizer = jsonArea["fertilizer"];

    sscanf(jsonArea["openTime"], "%hhu.%hhu.%hhu %hhu:%hhu:%hhu",
           &area.openTime.day, &area.openTime.month, &area.openTime.year,
           &area.openTime.hour, &area.openTime.minute, &area.openTime.secound);

    area.plansLen = 0;
    area.plans = nullptr;

    DEBUG_MODE_PRINT_VALUES("\tArea ===");
    DEBUG_MODE_PRINT_VALUES("\tclose: ", area.close);
    DEBUG_MODE_PRINT_VALUES("\tisOpen ", area.isOpen);
    DEBUG_MODE_PRINT_VALUES("\tactivePlan ", area.activePlan);
    DEBUG_MODE_PRINT_VALUES("\tmanualOpen ", area.manualOpen);
    DEBUG_MODE_PRINT_VALUES("\tfertilizer ", area.fertilizer);
    DEBUG_MODE_PRINT_VALUES("\topenTime [dd.mm.yy hh:mm:ss] ", area.openTime.day, ".", area.openTime.month, ".20", area.openTime.year, " ",
                            area.openTime.hour, ":", area.openTime.minute, ":", area.openTime.secound);

    JsonArray plansJson = jsonArea["plans"];
    if (plansJson)
    {
        area.plansLen = plansJson.size();
        area.plans = new Plan[area.plansLen];
        uint i = 0;
        for (JsonObject plan : plansJson)
        {
            setPlan(area.plans[i++], plan);
        }
    }
}

void UserData::setPlan(Plan &plan, const JsonObject &jsonPlan)
{
    plan.duration = jsonPlan["duration"];
    plan.repeatMethod = jsonPlan["repeatMethod"];
    sscanf(jsonPlan["startTime"], "%hhu:%hhu", &plan.startTime.hour, &plan.startTime.minute);
    const char *lastTime = jsonPlan["lastTime"] | "1.1.23";
    sscanf(lastTime, "%hhu.%hhu.%hhu", &plan.lastTime.day, &plan.lastTime.month, &plan.lastTime.year);

    JsonArray daysJson;
    switch (plan.repeatMethod)
    {
    case Plan::REPEAT_METHOD_DAILY:
        plan.repaet = jsonPlan["repeat"];
        break;
    case Plan::REPEAT_METHOD_WEEKLY:
        plan.repaet = 0;
        daysJson = jsonPlan["repeat"];
        if (daysJson)
        {
            for (uint8 day : daysJson)
            {
                SET_BIT(plan.repaet, day);
            }
        }
        break;
    case Plan::REPEAT_METHOD_NO_REPEAT:
    default:
        plan.repaet = 0;
        break;
    }

    DEBUG_MODE_PRINT_VALUES("\t\tPlan ===");
    DEBUG_MODE_PRINT_VALUES("\t\tstartTime: [hh:mm] ", plan.startTime.hour, ":", plan.startTime.minute);
    DEBUG_MODE_PRINT_VALUES("\t\tlastTime [dd.mm.yy] ", plan.lastTime.day, ".", plan.lastTime.month, ".20", plan.lastTime.year);
    DEBUG_MODE_PRINT_VALUES("\t\trepeatMethod ", plan.repeatMethod);
    DEBUG_MODE_PRINT_VALUES("\t\trepaet ", plan.repaet);
    DEBUG_MODE_PRINT_VALUES("\t\tduration ", plan.duration);
}

void UserData::setSetting(const JsonObject &jsonSetting)
{
    setting.maxTaps = jsonSetting["maxTaps"];
    setting.updateMethod = jsonSetting["updateMethod"];
    setting.UTC = 3; // jsonSetting["updateDelaySec"];
    DEBUG_MODE_PRINT_VALUES("\tSetting ===");
    DEBUG_MODE_PRINT_VALUES("\tmaxTaps: ", setting.maxTaps);
    DEBUG_MODE_PRINT_VALUES("\tupdateMethod: ", setting.updateMethod);
    DEBUG_MODE_PRINT_VALUES("\tUTC: ", setting.UTC);
}

void UserData::freeHeapMemory()
{
    for (uint8 i = 0; i < areasLen; i++)
    {
        delete areas[i].plans;
    }
    delete areas;

    areas = nullptr;
    areasLen = 0;
}

UserData::UserData()
{
    areas = nullptr;
    areasLen = 0;
}

UserData::~UserData()
{
    if (instance)
    {
        freeHeapMemory();
        delete instance;
        instance = nullptr;
    }
}

void UserData::update(const JsonObject &jsonUserData)
{

    freeHeapMemory();
    JsonArray jsonArrayAreas = jsonUserData["areas"];
    if (jsonArrayAreas)
    {
        areasLen = jsonArrayAreas.size();
        areas = new Area[areasLen];
        uint8 i = 0;
        for (JsonObject area : jsonArrayAreas)
        {
            setArea(areas[i++], area);
            // DEBUG_MODE_PRINT_NAMES_VALUES(i, areasLen);
        }
    }

    setSetting(jsonUserData["setting"]);
    // DEBUG_MODE_PRINT_NAMES_VALUES(areasLen, areas[1].plans[1].duration, setting.maxTaps);
}

uint8 UserData::getAreasLen() const
{
    return areasLen;
}

UserData &UserData::getInstance()
{
    if (!instance)
    {
        instance = new UserData();
    }
    return *instance;
}

#endif // ARDUINO