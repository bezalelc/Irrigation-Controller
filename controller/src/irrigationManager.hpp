#if defined(ARDUINO) && !defined(IRRIGATION_MANAGER_HPP)
#define IRRIGATION_MANAGER_HPP

#include <ctime>
#include <Arduino.h>
#include "userData.hpp"
#include "configData.hpp"
#include "network.hpp"
#include "firebaseHandler.hpp"
#include "periorityQueue.hpp"

class IrrigationManager
{
public:
    struct PlanTime
    {
        time_t nextTime;
        uint8 area;
        uint8 plan;
        bool isOpen;

        struct Update
        {
            time_t nextTime;
            bool isOpen;
        };

        PlanTime(time_t nextTime, uint8 area, uint8 plan, bool isOpen) : nextTime(nextTime), area(area), plan(plan), isOpen(isOpen) {}
    };

    IrrigationManager(Network *network, ConfigData &configData);

    ~IrrigationManager();

    bool isOpennedTaps() const;

    time_t scanForNext();

private:
    UserData &userData;
    FirebaseHandler &firebaseHandler;
    Network *network;
    PeriorityQueue<PlanTime, PlanTime::Update &> *plansQueue;
    uint8 opennedTaps;
    uint32 defaultDelay;
    time_t nextReading;

    void init();
    bool openArea(uint8 areaId, const struct tm &currentTimeTm, uint8 activePlan);
    bool closeArea(uint8 areaId);
    bool startPlan(const PlanTime &planTime, time_t currentTime);
    bool stopPlan(const PlanTime &planTime);
    void buildQueue();
    time_t calculateNextPlanTime(UserData::Plan &plan, time_t currentTime);
};

#endif // defined(ARDUINO) && !defined(IRRIGATION_MANAGER_HPP)
