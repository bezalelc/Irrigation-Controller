// defined(ARDUINO) && !defined(FIREBASE_HANDLER_HPP)
#ifndef HAL_HPP
#define HAL_HPP

#ifdef ESP32
#define CONFIG_INPUT_PIN 15 // todo
#elif ESP8266
#define CONFIG_INPUT_PIN D2
#endif // ESP32 or ESP8266

#endif // HAL_HPP