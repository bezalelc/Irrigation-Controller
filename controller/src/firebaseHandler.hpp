#if defined(ARDUINO) && !defined(FIREBASE_HANDLER_HPP)
#define FIREBASE_HANDLER_HPP

#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>
#include "configData.hpp"

#define JSON_MAX_SIZE 2049

class FirebaseHandler
{
public:
    static FirebaseHandler &getInstance(ConfigData &configData);

    void begin();
    bool readUserData(DynamicJsonDocument &doc, bool update = false);
    bool updateArea(uint8 areaId, const char *jsonBuff);
    bool deleteArea(uint8 areaId);
    bool updatePlanLastTime(uint8 areaId, uint8 planId, const char *lastTime);
    bool deletePlan(uint8 areaId, uint8 planId);
    ~FirebaseHandler();

private:
    FirebaseData firebaseData;
    FirebaseAuth firebaseAuth;
    FirebaseConfig firebaseConfig;

    String databasePath;
    const ConfigData &configData;
    static FirebaseHandler *instance;

    FirebaseHandler(const ConfigData &configData);

    template <typename... Args>
    inline const char *getPath(const Args &...args)
    {
        String path = databasePath;
        (path += ... += String(args));
        return path.c_str();
    }
    // template <typename... Args>
    // const char *getPath(Args... args)
    // {
    //     // Create a string stream to concatenate the path
    //     String path;
    //     appendPath(path, args...);
    //     // Return the concatenated path as a C-style string
    //     return path.c_str();
    // }
    // // Helper function for variadic template recursion
    // template <typename T, typename... Rest>
    // void appendPath(String &path, T &&arg, Rest &&...rest)
    // {
    //     path += '/';
    //     path += std::forward<T>(arg);
    //     appendPath(path, std::forward<Rest>(rest)...);
    // }
    // // Base case for the recursion
    // void appendPath(String &path)
    // {
    //     // Do nothing, this is the end of recursion
    // }

    bool deleteNode(const char *nodePath);
    bool updateNode(const char *nodePath, const char *jsonBuffern);
    bool updateStr(const char *strPath, const char *str);

    // Private copy constructor and assignment operator to prevent copying
    FirebaseHandler(const FirebaseHandler &) = delete;
    FirebaseHandler &operator=(const FirebaseHandler &) = delete;
};

#endif // defined(ARDUINO) && !defined(FIREBASE_HANDLER_HPP)