#if defined(ARDUINO) && !defined(IRRIGATION_MANAGER_HPP)
#define IRRIGATION_MANAGER_HPP

#include <ctime>
#include <Arduino.h>
#include "setting.hpp"
#include "area.hpp"
#include "configData.hpp"
#include "network.hpp"
#include "firebaseHandler.hpp"
#include "periorityQueue.hpp"

constexpr uint8 MAX_AREAS = 32;
constexpr uint16 MAX_TOTAL_PLANS = MAX_AREAS * MAX_PLANS_IN_AREA;

class IrrigationManager
{
public:
    struct PlanTime
    {
        time_t nextTime;
        uint8 areaId;
        uint8 planId;
        bool isOpen;

        struct Update
        {
            time_t nextTime;
            bool isOpen;
        };

        PlanTime(time_t nextTime, uint8 areaId, uint8 planId, bool isOpen)
            : nextTime(nextTime), areaId(areaId), planId(planId), isOpen(isOpen) {}
    };

    IrrigationManager(Network *network, FirebaseHandler &firebaseHandler);
    ~IrrigationManager();
    bool isOpennedTaps() const;
    time_t scanForNext();

    void addToQueue(uint8 areaId, uint8 planId);
    void deleteFromQueue(uint8 areaId, uint8 planId);
    void increasOpennedTaps();
    void decreasOpennedTaps();
    bool freeTap() const;

private:
    FirebaseHandler &firebaseHandler;
    Network *network;
    PeriorityQueue<PlanTime, PlanTime::Update &, MAX_TOTAL_PLANS> *plansQueue;
    uint8 opennedTaps;
    time_t nextReading;
    bool firebaseInit;

    Setting setting;
    Area *areas[MAX_AREAS];

    bool getFromFirebase();
    void buildQueue();
    time_t calculateNextPlanTime(Plan &plan, time_t currentTime);
};

#endif // defined(ARDUINO) && !defined(IRRIGATION_MANAGER_HPP)
