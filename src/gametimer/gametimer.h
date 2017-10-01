#ifndef GAME_TIMER_H
#define GAME_TIMER_H

#include "timeutil.h"

#define EVENT_ON_TIMER_FINISH -1

template<class FuncOwner> class GameTimer {
public:

    GameTimer() {
        mTotalTicks = mEventTicks = -1;
        reset();
    }

    GameTimer(  int totalTimeMS,
                FuncOwner *instance = NULL,
                void (FuncOwner::*eventFunc)() = NULL,
                int eventTimeMS = EVENT_ON_TIMER_FINISH) {
        initialize(totalTimeMS, instance, eventFunc, eventTimeMS);
    }

    void initialize(int totalTimeMS,
                    FuncOwner *instance = NULL,
                    void (FuncOwner::*eventFunc)() = NULL,
                    int eventTimeMS = EVENT_ON_TIMER_FINISH) {
        // Converts millis to game updates

        mTotalTicks = TimeUtil::millisToUpdates(totalTimeMS);

        if (eventTimeMS == EVENT_ON_TIMER_FINISH)
            mEventTicks = mTotalTicks;
        else
            mEventTicks = TimeUtil::millisToUpdates(eventTimeMS);

        mEventFunc = eventFunc;

        mEventFuncOwnerInstance = instance;

        reset();
    }

    void reset() {
        mCurrentTotalTicks = 0;
        mCurrentEventTicks = 0;
        mRunning = false;;
    }

    void restart() {
        reset();
        start();
    }

    void start() {
        mRunning = true;
    }

    void stop() {
        mRunning = false;
    }

    int update() {
        if (isRunning()) {
            if (mCurrentTotalTicks < mTotalTicks) {
                mCurrentTotalTicks++;
                if (mCurrentTotalTicks % mEventTicks == 0 &&
                    mEventFuncOwnerInstance != NULL &&
                    mEventFunc != NULL) {
                    (mEventFuncOwnerInstance->*mEventFunc)();
                }
            }
            else
                stop();
        }
        return mCurrentTotalTicks;
    }

    bool isRunning() {
        return mRunning;
    }

    int currentTicks() {
        return mCurrentTotalTicks;
    }

private:
    int mTotalTicks;
    int mEventTicks;

    int mCurrentTotalTicks;
    int mCurrentEventTicks;

    bool mRunning;

    FuncOwner *mEventFuncOwnerInstance;
    void (FuncOwner::*mEventFunc)();
};

#endif // GAME_TIMER_H