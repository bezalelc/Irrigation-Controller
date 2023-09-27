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

        PlanTime(time_t nextTime, uint8 area, uint8 plan) : nextTime(nextTime), area(area), plan(plan) {}
    };

    IrrigationManager(Network *network, ConfigData &configData);

    ~IrrigationManager();

    uint32 getNextDelay();

    bool isOpennedTaps() const;

    time_t scanForNext();

private:
    UserData &userData;
    FirebaseHandler &firebaseHandler;
    Network *network;
    PeriorityQueue<PlanTime, time_t> *plansQueue;
    uint8 opennedTaps;
    uint32 defaultDelay;
    time_t nextReading;

    bool openArea(const PlanTime &planTime, time_t currentTime);
    void buildQueue();
};

#endif // defined(ARDUINO) && !defined(IRRIGATION_MANAGER_HPP)
