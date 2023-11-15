#ifndef MAIN_CPP
#define MAIN_CPP

#ifdef ARDUINO

#include <Arduino.h>
#include <ctime>
#include "utils.h"
#include "debugUtils.hpp"
#include "irrigationManager.hpp"
#include "configData.hpp"
#include "network.hpp"
#include "firebaseHandler.hpp"

// delay for update firebase
#define DELAY 2000U

#if DEBUG_MODE
char *stackStart;
#endif

ConfigData configData;
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

  configData.init();

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

  // DEBUG_MODE_PRINT_MEMORY_USAGE;
  DEBUG_MODE_PRINT_NAMES_VALUES(delaySec);
  delay(SECOUNDS_TO_MILLIS(delaySec) + DELAY);
}

void restartConnection()
{
  network->connectToWifi();
  firebaseHandler->begin();
}

#else  // for native platform
int main()
{
  return 0;
}
#endif // ARDUINO
#endif // MAIN_CPP
