#ifdef ARDUINO

#include <ArduinoJson.h>
// Provide the token generation process info.
#include "addons/TokenHelper.h"
#include "config.h"
#include "debugUtils.hpp"
#include "network.hpp"
#include "firebaseHandler.hpp"

#define FIREBASE_MAIN_PATH "/users/"
#define FIREBASE_LAST_TIME_KEY "/lastTime/"
#define OPEN_TIME_MAX_LEN 25 // todo 18
#define LAST_TIME_MAX_LEN 25 // todo 9
#define AREA_JSON_MAX_SIZE 512
#define PLAN_JSON_MAX_SIZE 64

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
    DEBUG_MODE_SERIAL_PRINTLN(" Connected!");
    String userId = firebaseAuth.token.uid.c_str();
    // Update database path
    databasePath = FIREBASE_MAIN_PATH + userId;
}

bool FirebaseHandler::readUserData(UserData &userData)
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

    const char *jsonStr = firebaseData.to<FirebaseJson>().raw();
    DynamicJsonDocument doc(2048);
    DeserializationError err = deserializeJson(doc, jsonStr);
    if (err)
    {
        DEBUG_MODE_PRINT_VALUES(F("deserializeJson() failed with code "), err.f_str());
        return false;
    }
    JsonObject obj = doc.as<JsonObject>();
    userData.update(obj);
    doc.clear();

    return true;
}

bool FirebaseHandler::updatePlanStartTime(const UserData::Plan &plan, uint8 areaId, uint8 planId)
{
    if (!Firebase.ready())
    {
        DEBUG_MODE_PRINT_VALUES("Firebase.ready()=false");
        return false;
    }

    char timeBuffer[LAST_TIME_MAX_LEN];
    snprintf(timeBuffer, LAST_TIME_MAX_LEN, "%02d.%02d.%02d", plan.lastTime.day, plan.lastTime.month, plan.lastTime.year);

    if (Firebase.RTDB.setString(&firebaseData, databasePath + "/areas/" + areaId + "/plans/" + planId + FIREBASE_LAST_TIME_KEY, timeBuffer))
    {
        return true;
    }

    return false;
}

bool FirebaseHandler::updateOpenArea(const UserData::Area &area, uint8 areaId)
{
    if (!Firebase.ready())
    {
        DEBUG_MODE_PRINT_VALUES("Firebase.ready()=false");
        return false;
    }

    StaticJsonDocument<AREA_JSON_MAX_SIZE> areaJson;
    char timeBuffer[OPEN_TIME_MAX_LEN];

    areaJson["activePlan"] = area.activePlan;
    areaJson["close"] = false;
    areaJson["isOpen"] = area.isOpen;
    // areaJson["manualOpen"] = area.manualOpen;
    snprintf(timeBuffer, OPEN_TIME_MAX_LEN, "%02d.%02d.%02d %02d:%02d:%02d",
             area.openTime.day, area.openTime.month, area.openTime.year, area.openTime.hour, area.openTime.minute, area.openTime.secound);
    areaJson["openTime"] = timeBuffer;

    char jsonBuffer[AREA_JSON_MAX_SIZE];
    serializeJson(areaJson, jsonBuffer, AREA_JSON_MAX_SIZE);

    FirebaseJson json;
    json.setJsonData(jsonBuffer);
    if (Firebase.RTDB.updateNode(&firebaseData, databasePath + "/areas/" + areaId + "/", &json))
    {
        return true;
    }

    return false;
}

bool FirebaseHandler::updateCloseArea(uint8 areaId)
{
    if (!Firebase.ready())
    {
        DEBUG_MODE_PRINT_VALUES("Firebase.ready()=false");
        return false;
    }

    StaticJsonDocument<AREA_JSON_MAX_SIZE> areaJson;
    char timeBuffer[OPEN_TIME_MAX_LEN];

    areaJson["activePlan"] = -1;
    areaJson["close"] = false;
    areaJson["isOpen"] = false;
    areaJson["manualOpen"] = false;

    char jsonBuffer[AREA_JSON_MAX_SIZE];
    serializeJson(areaJson, jsonBuffer, AREA_JSON_MAX_SIZE);

    FirebaseJson json;
    json.setJsonData(jsonBuffer);
    if (Firebase.RTDB.updateNode(&firebaseData, databasePath + "/areas/" + areaId + "/", &json))
    {
        return true;
    }

    return false;
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
