template<typename FuncOwner>
GameTimer<FuncOwner>::GameTimer() {
    mTotalTicks = mEventTicks = -1;
    reset();
}

template<typename FuncOwner>
GameTimer<FuncOwner>::GameTimer(
            int totalTimeMS, FuncOwner *instance,
            void (FuncOwner::*eventFunc)(), int eventTimeMS) {
    initialize(totalTimeMS, instance, eventFunc, eventTimeMS);
}


template<typename FuncOwner>
void GameTimer<FuncOwner>::initialize(
    int totalTimeMS, FuncOwner *instance,
    void (FuncOwner::*eventFunc)(), int eventTimeMS) {
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

template<typename FuncOwner>
void GameTimer<FuncOwner>::start() {
    mRunning = true;
}

template<typename FuncOwner>
void GameTimer<FuncOwner>::stop() {
    mRunning = false;
}

template<typename FuncOwner>
void GameTimer<FuncOwner>::reset() {
    mCurrentTotalTicks = 0;
    stop();
}

template<typename FuncOwner>
void GameTimer<FuncOwner>::restart() {
    reset();
    start();
}

template<typename FuncOwner>
int GameTimer<FuncOwner>::update() {
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
    return currentTicks();
}

template<typename FuncOwner>
bool GameTimer<FuncOwner>::isRunning() {
    return mRunning;
}

template<typename FuncOwner>
int GameTimer<FuncOwner>::currentTicks() {
    return mCurrentTotalTicks;
}