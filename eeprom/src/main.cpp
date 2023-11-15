#include <Arduino.h>
#include <EEPROM.h>

#define BUFF_MAX_LEN 33

typedef enum CONFIG_DATA_TYPE {
  CONFIG_DATA_TYPE_WIFI_SSID,
  CONFIG_DATA_TYPE_WIFI_PASSWORD,
  CONFIG_DATA_TYPE_AP_SSID,
  CONFIG_DATA_TYPE_AP_PASSWORD,
  CONFIG_DATA_TYPE_USER_NAME,
  CONFIG_DATA_TYPE_USER_PASSWORD,
  CONFIG_DATA_TYPE_MAX_VAL,
} CONFIG_DATA_TYPE;

struct ConfigData {
  char wifiSSID[BUFF_MAX_LEN];
  char wifiPassword[BUFF_MAX_LEN];
  char apSSID[BUFF_MAX_LEN];
  char apPassword[BUFF_MAX_LEN];
  char userUser[BUFF_MAX_LEN];
  char userPassword[BUFF_MAX_LEN];
};
ConfigData configData;

void readConfigData() {
  uint8_t* dataPtr = reinterpret_cast<uint8_t*>(&configData);

  for (uint16_t address = 0; address < sizeof(configData); address++) {
    dataPtr[address] = EEPROM.read(address);
  }

  Serial.print("[readConfigData] wifi ssid: |");
  Serial.print(configData.wifiSSID);
  Serial.println("|");
  Serial.print("[readConfigData] wifi pass: |");
  Serial.print(configData.wifiPassword);
  Serial.println("|");
  Serial.print("[readConfigData] ap   ssid: |");
  Serial.print(configData.apSSID);
  Serial.println("|");
  Serial.print("[readConfigData] ap   pass: |");
  Serial.print(configData.apPassword);
  Serial.println("|");
  Serial.print("[readConfigData] user user: |");
  Serial.print(configData.userUser);
  Serial.println("|");
  Serial.print("[readConfigData] user pass: |");
  Serial.print(configData.userPassword);
  Serial.println("|");
}


void setup() {
  Serial.begin(115200);
  Serial.println("------------   [setup]   ----------");
  EEPROM.begin(sizeof(configData));

  // strncpy(configData.wifiSSID, "BiBi", sizeof("BiBi") + 1);
  // strncpy(configData.wifiPassword, "yehr7ey3", sizeof("yehr7ey3") + 1);
  strncpy(configData.wifiSSID, "Bezalel&Adi", sizeof("Bezalel&Adi") + 1);
  strncpy(configData.wifiPassword, "liberman56", sizeof("liberman56") + 1);
  strncpy(configData.apSSID, "thermometer", sizeof("thermometer") + 1);
  strncpy(configData.apPassword, "thermometer", sizeof("thermometer") + 1);
  strncpy(configData.userUser, "bezalelcohen1@gmail.com", sizeof("bezalelcohen1@gmail.com") + 1);
  strncpy(configData.userPassword, "123456", sizeof("123456") + 1);


  uint8_t* dataPtr = reinterpret_cast<uint8_t*>(&configData);
  for (uint8_t address = 0; address < sizeof(configData); address++) {
    EEPROM.write(address, dataPtr[address]);
  }
  EEPROM.commit();
}

void loop() {
  // uint8_t* dataPtr = reinterpret_cast<uint8_t*>(&configData);

  // for (uint16_t address = 0; address < sizeof(ConfigData); address++) {
  //   dataPtr[address] = EEPROM.read(address);
  // }
    readConfigData();


  // Serial.printf("user=|%s|\npass=|%s|\n", configData.wifiSSID, configData.wifiPassword);
}