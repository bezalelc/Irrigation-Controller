#ifndef UTILS_HPP
#define UTILS_HPP

#define SET_BIT(variable, bit) (variable |= (1 << bit))
#define IS_BIT_SET(variable, bit) ((variable & (1 << bit)) != 0)

#define MAX_UINT32 (0xFFFFFFFFU)

#define MINUTES_TO_SECOUNDS(minutes) (minutes * 60)
#define HOURS_TO_SECOUNDS(hours) (hours * MINUTES_TO_SECOUNDS(60))
#define DAYS_TO_SECOUNDS(days) (days * HOURS_TO_SECOUNDS(24))
#endif
