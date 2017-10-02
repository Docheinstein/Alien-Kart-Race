#ifndef GAME_TIMER_H
#define GAME_TIMER_H

#include "timeutil.h"

#define EVENT_ON_TIMER_FINISH -1

/**
 * Timer based on the updates of the game that can calls a given function.
 * The timer counts the elapsed milliseconds indipendently from the FPS of the
 * game, so the amount of runnig time will be the same under different FPS rate.
 * The timer can be used as timer only, or can be initialized with a function pointer,
 * in that case it will call the given function with a certain frequency specified
 * in initialization.
 */
template<typename FuncOwner> class GameTimer {
public:

    /** Creates an invalid and timer. */
    GameTimer();

    /**
     * Creates a game timer.
     * @param totalTimeMS   the time in millis this timer will reach.
     * @param instance      the instance of the entity that owns the given member function
     * @param eventFunc     the function pointer that will be called every eventTimeMS
     * @param eventTimeMS   the time in millis that specify how often call the given function pointer.
     *                      If not specified, the function pointer will be called when the timer finishes
     *                      the total millis count.
     */
    GameTimer(  int totalTimeMS,
                FuncOwner *instance = NULL,
                void (FuncOwner::*eventFunc)() = NULL,
                int eventTimeMS = EVENT_ON_TIMER_FINISH);

    /**
     * Initializes the game timer.
     * @param totalTimeMS   the time in millis this timer will reach.
     * @param instance      the instance of the entity that owns the given member function
     * @param eventFunc     the function pointer that will be called every eventTimeMS
     * @param eventTimeMS   the time in millis that specify how often call the given function pointer.
     *                      If not specified, the function pointer will be called when the timer finishes
     *                      the total millis count.
     */
    void initialize(int totalTimeMS,
                    FuncOwner *instance = NULL,
                    void (FuncOwner::*eventFunc)() = NULL,
                    int eventTimeMS = EVENT_ON_TIMER_FINISH);

    /**
     * Starts the timer.
     * Note that this call just set the running flag to true,
     * but in order to let the timer go forward, #update must be called.
     */
    void start();

    /**
     * Stops the timer.
     * Any call to #update done after the timer is stopper will be ignored.
     */
    void stop();

    /**
     * Invalidates the timer by setting the elapsed ticks to 0 and stops it.
     * The instance and the function pointer are kept valid, so the timer can
     * be used again for fire the same function.
     * If there is need to use the timer with a different function, a new timer
     * should be instantiated.
     */
    void reset();

    /**
     * Invalidates the timer by setting the elapsed ticks to 0 and restart it.
     * This actually call #reset and #start
     */
    void restart();

    /**
     * Updates the timer by going forward of one tick.
     * The method won't do anything if the timer is not running.
     * The function this timer has been instantiated with will be called
     * if the wanted event time has passed since the last function call.
     * @return the current ticks (game update) elapsed from the initialization of the timer.
     */
    int update();

    /**
     * Returns whether the timer is running.
     * @return whether the timer is running.
     */
    bool isRunning();

    /**
     * Returns the ticks elapsed from the initialization of the timer.
     * @return the ticks elapsed from the initialization of the timer.
     */
    int currentTicks();

private:
    /** Total ticks this timer will count. */
    int mTotalTicks;

    /** Ticks that will elapse between every function call. */
    int mEventTicks;

    /** Current ticks elapsed from the beginning of the timer. */
    int mCurrentTotalTicks;

    /** Whether this timer is running. */
    bool mRunning;

    /** Instance of the entity owner of the given function pointer. */
    FuncOwner *mEventFuncOwnerInstance;

    /** Function that will be called every #mEventTicks. */
    void (FuncOwner::*mEventFunc)();
};

#include "gametimer.tpp"

#endif // GAME_TIMER_H