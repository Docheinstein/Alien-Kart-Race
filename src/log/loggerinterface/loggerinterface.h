#ifndef LOGGER_INTERFACE_H
#define LOGGER_INTERFACE_H

#include "logger.h"

/**
 * Interface usable by entities that need to print on the output stream.
 * Use this interface is preferred respect to invoke directly the logger
 * method, since this automatically handle the log tag and whether the
 * entity can log or not.
 */
class LoggerInterface {
public:
    virtual ~LoggerInterface() {}

protected:
    /**
     * Prints a debug message using the default output verbosity level (1).
     * @param first the first argument.
     * @param other the orther arguments.
     */
    template<typename FirstType, typename ...OtherTypes>
    inline
    void d(FirstType && first, OtherTypes && ...other);

    /**
     * Prints a debug message using output verbosity level equals to 1
     * @param first the first argument.
     * @param other the orther arguments.
     */
    template<typename FirstType, typename ...OtherTypes>
    void d1(FirstType && first, OtherTypes && ...other);

    /**
     * Prints a debug message using output verbosity level equals to 2
     * @param first the first argument.
     * @param other the orther arguments.
     */
    template<typename FirstType, typename ...OtherTypes>
    void d2(FirstType && first, OtherTypes && ...other);

    /**
     * Prints a debug message using the given verbosity level.
     * @param level the verbosity debug level of this print.
     * @param first the first argument.
     * @param other the orther arguments.
     */
    template<typename FirstType, typename ...OtherTypes>
    void d_level(unsigned int level, FirstType && first, OtherTypes && ...other);

    /**
     * Prints a warn message.
     * @param first the first argument.
     * @param other the orther arguments.
     */
    template<typename FirstType, typename ...OtherTypes>
    void w(FirstType && first, OtherTypes && ...other);

    /**
     * Prints a bench message.
     * @param first the first argument.
     * @param other the orther arguments.
     */
    template<typename FirstType, typename ...OtherTypes>
    void bench(FirstType && first, OtherTypes && ...other);

    /**
    * Returns a tag that identifies the entity in the
    * output stream (the tag should possibilty be unique in order
    * to easily identifies the entities).
    * @return the tag used for identify this entity in the log.
    */
    virtual const char *logTag() = 0;

    /**
     * Returns whether this entity is able to print message to
     * the output stream.
     * If this method return false, every message won't be printed to
     * the output stream.
     * @return whether this entity can log.
     */
    virtual bool canLog() = 0;
};

#include "loggerinterface.tpp"

#endif // LOGGER_INTERFACE_H