#ifdef ARDUINO

// Provide the token generation process info.
#include "addons/TokenHelper.h"
#include "config.h"
#include "debugUtils.hpp"
#include "network.hpp"
#include "firebaseHandler.hpp"

#define FIREBASE_MAIN_PATH "/users/"
#define FIREBASE_UPDATE_PATH "/update/"
#define FIREBASE_AREAS_PATH "/areas/"
#define FIREBASE_PLANS_PATH "/plans/"
#define FIREBASE_LAST_TIME_KEY "/lastTime/"
#define AREA_JSON_MAX_SIZE 512

FirebaseHandler *FirebaseHandler::instance = nullptr;

FirebaseHandler::FirebaseHandler(const ConfigData &configData) : configData(configData)
{

    firebaseConfig.api_key = FIREBASE_API_KEY;
    firebaseConfig.database_url = FIREBASE_DATABASE_URL;

    // Assign the callback function for the long running token generation task */
    firebaseConfig.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    // Assign the maximum retry of token generation
    firebaseConfig.max_token_generation_retry = 5;
}

FirebaseHandler &FirebaseHandler::getInstance(ConfigData &configData)
{
    if (!instance)
    {
        instance = new FirebaseHandler(configData);
    }
    return *instance;
}

void FirebaseHandler::begin()
{
    firebaseAuth.user.email = &configData.getUsername();
    firebaseAuth.user.password = &configData.getUserPassword();

    Firebase.reconnectWiFi(true);
    // Optional, set number of error retry
    Firebase.RTDB.setMaxRetry(&firebaseData, 3);
    // Optional, set number of error resumable queues
    Firebase.RTDB.setMaxErrorQueue(&firebaseData, 30);
    // Optional, use classic HTTP GET and POST requests.
    // This option allows get and delete functions (PUT and DELETE HTTP requests) works for
    // device connected behind the Firewall that allows only GET and POST requests.
    // Firebase.RTDB.enableClassicRequest(&fbdo, true);
    // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
    // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
    // fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
    firebaseData.setResponseSize(4096);

    // Initialize the library with the Firebase authen and config
    Firebase.begin(&firebaseConfig, &firebaseAuth);

    // Getting the user UID might take a few seconds
    DEBUG_MODE_SERIAL_PRINT("Getting User UID ");
    while (!Network::getConfigMode() && (firebaseAuth.token.uid) == "")
    {
        DEBUG_MODE_SERIAL_PRINT('.');
        delay(1000);
    }
    DEBUG_MODE_SERIAL_PRINTLN(" Finish!");
    String userId = firebaseAuth.token.uid.c_str();
    // Update database path
    databasePath = FIREBASE_MAIN_PATH + userId;
}

bool FirebaseHandler::readUserData(DynamicJsonDocument &doc, bool update)
{
    if (!Firebase.ready())
    {
        DEBUG_MODE_PRINT_VALUES("Firebase.ready()=false");
        return false;
    }
    if (!Firebase.RTDB.getJSON(&firebaseData, databasePath))
    {
        DEBUG_MODE_PRINT_VALUES(firebaseData.errorReason().c_str());
        return false;
    }

    DeserializationError err = deserializeJson(doc, firebaseData.to<FirebaseJson>().raw());
    // DEBUG_MODE_PRINT_VALUES(jsonStr);
    // serializeJson(doc, Serial);
    if (err)
    {
        DEBUG_MODE_PRINT_VALUES(F("deserializeJson() failed with code "), err.f_str());
        return false;
    }

    return true;
}

bool FirebaseHandler::deleteNode(const char *nodePath)
{
    if (!Firebase.ready())
    {
        DEBUG_MODE_PRINT_VALUES("Firebase.ready()=false");
        return false;
    }

    if (Firebase.RTDB.deleteNode(&firebaseData, nodePath))
    {
        DEBUG_MODE_PRINT_VALUES(firebaseData.errorReason().c_str());
        return false;
    }

    return true;
}

bool FirebaseHandler::updateNode(const char *nodePath, const char *jsonBuffer)
{
    if (!Firebase.ready())
    {
        DEBUG_MODE_PRINT_VALUES("Firebase.ready()=false");
        return false;
    }

    FirebaseJson firebaseJson;
    firebaseJson.setJsonData(jsonBuffer);

    if (!Firebase.RTDB.updateNode(&firebaseData, nodePath, &firebaseJson))
    {
        DEBUG_MODE_PRINT_VALUES(firebaseData.errorReason().c_str());
        return false;
    }

    return true;
}

bool FirebaseHandler::updateStr(const char *strPath, const char *str)
{
    if (!Firebase.ready())
    {
        DEBUG_MODE_PRINT_VALUES("Firebase.ready()=false");
        return false;
    }
    DEBUG_MODE_PRINT_NAMES_VALUES(strPath, str);

    if (!Firebase.RTDB.setString(&firebaseData, strPath, str))
    {
        DEBUG_MODE_PRINT_VALUES(firebaseData.errorReason().c_str());
        return false;
    }

    return true;
}

bool FirebaseHandler::updateArea(uint8 areaId, const char *jsonBuff)
{
    DEBUG_MODE_PRINT_VALUES("line");
    return updateNode((databasePath + FIREBASE_AREAS_PATH + "id" + areaId).c_str(), jsonBuff);
}

bool FirebaseHandler::deleteArea(uint8 areaId)
{
    return deleteNode((databasePath + FIREBASE_AREAS_PATH + areaId).c_str());
}

bool FirebaseHandler::updatePlanLastTime(uint8 areaId, uint8 planId, const char *lastTime)
{
    DEBUG_MODE_PRINT_VALUES(lastTime);
    String path = databasePath + FIREBASE_AREAS_PATH + "id" + areaId + FIREBASE_PLANS_PATH + "id" + planId + FIREBASE_LAST_TIME_KEY;
    return updateStr(path.c_str(), lastTime);
}

bool FirebaseHandler::deletePlan(uint8 areaId, uint8 planId)
{
    return deleteNode((databasePath + FIREBASE_AREAS_PATH + areaId + FIREBASE_PLANS_PATH + planId).c_str());
}

FirebaseHandler::~FirebaseHandler()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

#endif // ARDUINO
