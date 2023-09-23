#if defined(ARDUINO) && !defined(FIREBASE_HANDLER_HPP)
#define FIREBASE_HANDLER_HPP

#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>
#include "configData.hpp"
#include "userData.hpp"

class FirebaseHandler
{
public:
    static FirebaseHandler &getInstance(ConfigData &configData);

    void begin();
    bool readUserData(UserData &userData);
    ~FirebaseHandler();

private:
    FirebaseData firebaseData;
    FirebaseAuth firebaseAuth;
    FirebaseConfig firebaseConfig;

    String databasePath;
    const ConfigData &configData;
    static FirebaseHandler *instance;

    FirebaseHandler(const ConfigData &configData);

    // bool readArea(FirebaseJson &area, FirebaseJsonData &jsonObj);
    // bool readPlan(FirebaseJson &plan, FirebaseJsonData &jsonObj);

    // Private copy constructor and assignment operator to prevent copying
    FirebaseHandler(const FirebaseHandler &) = delete;
    FirebaseHandler &operator=(const FirebaseHandler &) = delete;
};

#endif // defined(ARDUINO) && !defined(FIREBASE_HANDLER_HPP)