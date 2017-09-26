#include "timeutil.h"

int TimeUtil::millisToUpdates(int millis) {
    return  Game::TARGET_UPDATES_PER_SECOND * millis / 1000;
}

int TimeUtil::updatesToMillis(int updates) {
    return  updates * 1000 / Game::TARGET_UPDATES_PER_SECOND;
}