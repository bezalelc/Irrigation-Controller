#if defined(ARDUINO) && !defined(MAIN_CPP)
#define MAIN_CPP

#include <Arduino.h>
#include <ctime>
#include "tap.hpp"
#include "debugUtils.hpp"
#include "userData.hpp"
#include "configData.hpp"
#include "network.hpp"
#include "firebaseHandler.hpp"
#include "irrigationManager.hpp"

// delay for update firebase
#define DELAY 10000U

ConfigData configData;
UserData *userData;
Network *network;
FirebaseHandler *firebaseHandler;
IrrigationManager *irrigationManager;

void restartConnection();

void setup()
{
  DEBUG_MODE_SERIAL_BEGIN;

  userData = &UserData::getInstance();
  network = &Network::getInstance(configData);
  firebaseHandler = &FirebaseHandler::getInstance(configData);
  irrigationManager = new IrrigationManager(network, configData);

  if (!network->getConfigMode())
  {
    restartConnection();
  }
}

void loop()
{
  if (network->getConfigMode())
  {
    network->APMode();
    restartConnection();
  }
  else
  {
    // irrigationManager->buildQueue();
    irrigationManager->scanForNext();
    // irrigationManager->getNextDelay();
  }

  DEBUG_MODE_PRINT_MEMORY_USAGE;
  delay(DELAY);
}

void restartConnection()
{
  network->connectToWifi();
  firebaseHandler->begin();
}

#elif !defined(ARDUINO) && !defined(MAIN_CPP)
int main()
{
  return 0;
}
#endif // defined(ARDUINO) && !defined(MAIN_CPP)
