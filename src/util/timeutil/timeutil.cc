#include "timeutil.h"
#include "const.h"

int TimeUtil::millisToUpdates(int millis) {
    return  Const::TARGET_UPDATES_PER_SECOND * millis / 1000;
}

int TimeUtil::updatesToMillis(int updates) {
    return  updates * 1000 / Const::TARGET_UPDATES_PER_SECOND;
}