#ifndef UTILS_HPP
#define UTILS_HPP

#define SET_BIT(variable, bit) (variable |= (1 << bit))
#define IS_BIT_SET(variable, bit) ((variable & (1 << bit)) != 0)

#define MAX_UINT32 (0xFFFFFFFFU)

#define MINUTES_TO_SECOUNDS(minutes) (minutes * 60)
#define HOURS_TO_SECOUNDS(hours) (hours * MINUTES_TO_SECOUNDS(60))
#define DAYS_TO_SECOUNDS(days) (days * HOURS_TO_SECOUNDS(24))
#define SECOUNDS_TO_MILLIS(secounds) (secounds * 1000)

#ifdef ESP32
typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
#endif

#endif
