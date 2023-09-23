#if defined(ARDUINO) && !defined(TAP_HPP)
#define TAP_HPP

#include <Arduino.h>

class Tap
{
private:
    /* data */
public:
    Tap(/* args */);
    ~Tap();
    void open(uint8 tap)
    {
    }

    void close(uint8 tap)
    {
    }
};



#endif // defined(ARDUINO) && !defined(TAP_HPP)