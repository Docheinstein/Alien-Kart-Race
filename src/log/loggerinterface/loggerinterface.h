#ifndef LOGGER_INTERFACE_H
#define LOGGER_INTERFACE_H

#include "logger.h"

class LoggerInterface {
public:
    virtual ~LoggerInterface() {}

protected:
    template<typename FirstType, typename ...OtherTypes>
    inline
    void d_level(unsigned int level, FirstType && first, OtherTypes && ...other) {
        if (canLog())
            debug(level, logTag(), "\t", first, other...);
    }

    template<typename FirstType, typename ...OtherTypes>
    inline
    void d(FirstType && first, OtherTypes && ...other) {
        d_level(1, first, other...);
    }

    template<typename FirstType, typename ...OtherTypes>
    inline
    void d1(FirstType && first, OtherTypes && ...other) {
        d_level(1, first, other...);
    }

    template<typename FirstType, typename ...OtherTypes>
    inline
    void d2(FirstType && first, OtherTypes && ...other) {
        d_level(2, first, other...);
    }

    template<typename FirstType, typename ...OtherTypes>
    inline
    void w(FirstType && first, OtherTypes && ...other) {
        if (canLog())
            warn(logTag(), "\t", first, other...);
    }

    template<typename FirstType, typename ...OtherTypes>
    inline
    void bench(FirstType && first, OtherTypes && ...other) {
        if (canLog())
            benchmark(logTag(), "\t", first, other...);
    }

    virtual const char *logTag() = 0;
    virtual bool canLog() = 0;
};

#endif // LOGGER_INTERFACE_H