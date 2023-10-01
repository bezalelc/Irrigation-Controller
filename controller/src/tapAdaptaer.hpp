#if defined(ARDUINO) && !defined(TAP_ADAPTAER_HPP)
#define TAP_ADAPTAER_HPP

#include <Arduino.h>

class TapAdapter
{
private:
public:
    TapAdapter();
    ~TapAdapter();
    static void open(uint8 tap);
    static void close(uint8 tap);
};

#endif // defined(ARDUINO) && !defined(TAP_ADAPTAER_HPP)