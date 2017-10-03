#ifndef TIME_UTIL_H
#define TIME_UTIL_H

/** Provides utility methods for the time. */
class TimeUtil {
public:
    /**
     * Returns the updates that correspond with the given millis time for the
     * target updates per seconds (Const#TARGET_UPDATES_PER_SECOND).
     * @param  millis the time in millis.
     * @return        the time converted in updates.
     */
    static int millisToUpdates(int millis);

    /**
     * Returns the millis that correspond with the given updates for the
     * target updates per seconds (Const#TARGET_UPDATES_PER_SECOND).
     * @param  updates the updates count.
     * @return         the updates converted in millis.
     */
    static int updatesToMillis(int updates);
};

#endif