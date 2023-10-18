#if defined(ARDUINO) && !defined(MAIN_CPP)
#define MAIN_CPP

#include <Arduino.h>
#include <ctime>
#include "irrigationManager.hpp"
#include "debugUtils.hpp"
#include "configData.hpp"
#include "network.hpp"
#include "firebaseHandler.hpp"

// delay for update firebase
#define DELAY 2000U

#if DEBUG_MODE
char *stackStart;
#endif

ConfigData configData;
// UserData *userData;
Network *network;
FirebaseHandler *firebaseHandler;
IrrigationManager *irrigationManager;
time_t delaySec;

void restartConnection();

void setup()
{
#if DEBUG_MODE
  char stack;
  stackStart = &stack;
#endif // DEBUG_MODE

  DEBUG_MODE_SERIAL_BEGIN;

  network = &Network::getInstance(configData);
  firebaseHandler = &FirebaseHandler::getInstance(configData);
  irrigationManager = new IrrigationManager(network, *firebaseHandler);

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
    delaySec = irrigationManager->scanForNext();
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
