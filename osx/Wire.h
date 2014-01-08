
#ifndef Wire_H
#define Wire_H

#include <Arduino.h>

class WireClass
{
  public:
    WireClass() {};
    static void begin() {};
};


extern WireClass Wire;

#endif
