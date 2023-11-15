#if defined(ARDUINO) && !defined(AREA_HPP)
#define AREA_HPP

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ctime>
#include "plan.hpp"

constexpr uint8 MAX_PLANS_IN_AREA = 0x4;
constexpr uint16 AREA_JSON_MAX_SIZE = 256;

class FirebaseHandler;
class IrrigationManager;

class Area
{
public:
    struct OpenTimeDate
    {
        uint8 year;
        uint8 month;
        uint8 day;
        uint8 hour;
        uint8 minute;
        uint8 secound;
    };

    // Area(uint8 id, IrrigationManager &irrigationManager, FirebaseHandler &firebaseHandler);
    Area(uint8 id, const JsonObject &jsonArea, IrrigationManager &irrigationManager, FirebaseHandler &firebaseHandler);
    ~Area();

    void update(const JsonObject &jsonArea, time_t currentTime);

    bool openPlan(time_t currentTime, int8 planId);
    bool closePlan(uint8 planId);
    void deletePlan(uint8 planId, bool deleteQueue = false);

    friend class IrrigationManager;
    friend class UserData;

private:
    uint8 id;
    IrrigationManager &irrigationManager;
    FirebaseHandler &firebaseHandler;

    bool close;
    bool isOpen;
    int8 activePlan; // 3bit options: [,7]
    bool manualOpen;
    bool fertilizer;
    OpenTimeDate openTime;

    Plan *plans[MAX_PLANS_IN_AREA];

    bool openManual(time_t currentTime);
    bool closeManual();
    bool closeNow();

    void updateFirebase(bool addNew);

    void openTap(time_t currentTime);
    void closeTap();
};

#endif // defined(ARDUINO) && !defined(AREA_HPP)