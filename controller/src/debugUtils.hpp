#if defined(ARDUINO) && !defined(DEBUG_UTILS_HPP)
#define DEBUG_UTILS_HPP

/**
 * this file handle defines for debug mode:
 * when debug mode is enable - all the debug output will print to the serial through
 * USB connection
 * in production the flag DEBUG_MODE=0 and debug disable for saving CPU cycles
 */
#if DEBUG_MODE

#include <Arduino.h>

#define MONITOR_SPEED 115200U
#define DEBUG_MODE_SERIAL_BEGIN Serial.begin(MONITOR_SPEED)

#define DEBUG_MODE_SERIAL_PRINT(str) Serial.print(str)
#define DEBUG_MODE_SERIAL_PRINTLN(str) Serial.println(str)
#define DEBUG_MODE_SERIAL_PRINTF(...) Serial.printf(__VA_ARGS__)

/**
 * print prefix for debug mode with format: [filename:line][function]
 */
#define DEBUG_MODE_PREFIX DEBUG_MODE_SERIAL_PRINTF("[%s:%d][%s] ", __FILE__, __LINE__, __func__)

/**
 * print multiple values to serial:
 * DEBUG_MODE_PRINT_VALUES("arg", 56, 9.9);
 *
 * >>> [file name][function name] |arg| |56| |9.9|
 */
#define DEBUG_MODE_PRINT_VALUES(...)   \
    do                                 \
    {                                  \
        DEBUG_MODE_PREFIX;             \
        printValues(__VA_ARGS__);      \
        DEBUG_MODE_SERIAL_PRINT('\n'); \
    } while (0)

template <typename T>
static void printValues(T varValue)
{
    DEBUG_MODE_SERIAL_PRINT(varValue);
}

template <typename T, typename... Rest>
static void printValues(T varValue, Rest... rest)
{
    printValues(varValue);
    printValues(rest...);
}

/**
 * print multiple argument with arguments names to serial:
 * int x=1;
 * char c='l';
 * float f=9.9f;
 * DEBUG_MODE_PRINT_NAMES_VALUES(x, c, l);
 *
 * >>> [file name:line][function name] x=|1| c=|l| f=|9.9|
 */
#define DEBUG_MODE_PRINT_NAMES_VALUES(...)           \
    do                                               \
    {                                                \
        DEBUG_MODE_PREFIX;                           \
        printNamesValues(#__VA_ARGS__, __VA_ARGS__); \
        DEBUG_MODE_SERIAL_PRINT('\n');               \
    } while (0)

// base case for template recursion when one argument remains
template <typename Arg>
static void printNamesValues(const char *name, Arg &&arg)
{
    DEBUG_MODE_SERIAL_PRINTF("%s=|", name);
    DEBUG_MODE_SERIAL_PRINT(arg);
    DEBUG_MODE_SERIAL_PRINT("|");
}

// recursive variadic template for multiple arguments
template <typename Arg, typename... Rest>
static void printNamesValues(const char *names, Arg &&arg, Rest &&...args)
{
    while (*names != ',')
    {
        DEBUG_MODE_SERIAL_PRINT(*names);
        names++;
    }
    DEBUG_MODE_SERIAL_PRINT("=|");
    DEBUG_MODE_SERIAL_PRINT(arg);
    DEBUG_MODE_SERIAL_PRINT("|");
    printNamesValues(names + 1, args...);
}

#define DEBUG_MODE_PRINT_MEMORY_USAGE \
    DEBUG_MODE_PRINT_VALUES("FreeHeap=", ESP.getFreeHeap(), ", SketchSize=", ESP.getSketchSize(), ", FreeSketchSpace=", ESP.getFreeSketchSpace(), ", CpuFreqMHz=", ESP.getCpuFreqMHz(), ", stack size=", (int32_t)stackSize());
extern char *stackStart;
static int32_t stackSize()
{
    char stack;
    return (int32_t)stackStart - (int32_t)&stack;
}

#else
#define DEBUG_MODE_SERIAL_BEGIN
#define DEBUG_MODE_PREFIX
#define DEBUG_MODE_PRINT_VALUES(...)
#define DEBUG_MODE_PRINT_NAMES_VALUES(...)
#define DEBUG_MODE_SERIAL_PRINT(str)
#define DEBUG_MODE_SERIAL_PRINTLN(str)
#define DEBUG_MODE_SERIAL_PRINTF(...)
#define DEBUG_MODE_PRINT_MEMORY_USAGE
#endif // DEBUG_MODE

#endif // defined(ARDUINO) && !defined(DEBUG_UTILS_HPP)