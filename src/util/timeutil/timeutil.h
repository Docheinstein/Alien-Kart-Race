#ifndef TIME_UTIL_H
#define TIME_UTIL_H

#include "game.h"

class TimeUtil {
public:
    static int millisToUpdates(int millis);
    static int updatesToMillis(int updates);
};

#endif