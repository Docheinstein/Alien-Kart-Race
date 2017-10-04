#include "timeutil.h"
#include "config.h"

int TimeUtil::millisToUpdates(int millis) {
    return  Config::TARGET_UPDATES_PER_SECOND * millis / 1000;
}

int TimeUtil::updatesToMillis(int updates) {
    return  updates * 1000 / Config::TARGET_UPDATES_PER_SECOND;
}