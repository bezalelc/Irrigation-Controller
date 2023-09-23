#if defined(ARDUINO) && !defined(IRRIGATION_MANAGER_HPP)
#define IRRIGATION_MANAGER_HPP

#include <ctime>
#include <Arduino.h>
#include "userData.hpp"
#include "configData.hpp"
#include "network.hpp"
#include "firebaseHandler.hpp"

class IrrigationManager
{
private:
    UserData &userData;
    FirebaseHandler &firebaseHandler;
    Network *network;
    uint8 opennedTaps;
    uint32 defaultDelay;

    void openTap(uint8 tapIdx);

    static uint32 timeNotPass(const struct tm &currentTime, const UserData::Plan::StartTime &startTime);

    bool openArea(UserData::Area &area, uint planIdx, const struct tm &currentTime);

public:
    IrrigationManager(Network *network, ConfigData &configData);

    ~IrrigationManager();

    uint32 getNextDelay();
};

#endif // defined(ARDUINO) && !defined(IRRIGATION_MANAGER_HPP)
