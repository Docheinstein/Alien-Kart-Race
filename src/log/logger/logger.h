#ifndef LOGGER_H
#define LOGGER_H

// This file contains the functions used by the game for print to the
// output stream. Functions in there are global for output in a handly
// and faster way.

// -----------------------------------------------------------------
// DEBUG -----------------------------------------------------------
// -----------------------------------------------------------------

/**
* Prints a debug message by recursively output the variadic arguments.
* A level should be specified, and the message will be put on output
* only if the debug mode is enabled and the current debug level is
* compatible with the specified level (an AND bit a bit is done for this purpose).
* @param level 	the output level; higher is more verbose (actually 1 or 2 can be used).
* @param logTag tag of the entity that is printing this message.
* @param first 	the first argument.
* @param other 	the other arguments.
*/
template<typename FirstType, typename ...OtherTypes>
void debug(	unsigned int level, const char *logTag,
			FirstType && first, OtherTypes && ...other);

/**
 * Prints a debug message by recursively output the variadic arguments.
 * @param first the first argument.
 * @param other the other arguments.
 */
template<typename FirstType, typename ...OtherTypes>
void debug_internal(FirstType && first, OtherTypes && ...other);

/**
 * End function for the recursive debug call.
 * Prints end of line and eventually flushes the output.
 */
void debug_internal();

// -----------------------------------------------------------------
// WARN ------------------------------------------------------------
// -----------------------------------------------------------------


/**
 * Prints a warn message by recursively output the variadic arguments.
 * The message will be put on the output stream onlt if the warn mode is enabled.
 * @param logTag tag of the entity that is printing this message.
 * @param first the first argument.
 * @param other the other arguments.
 */
template<typename FirstType, typename ...OtherTypes>
void warn(const char *logTag, FirstType && first, OtherTypes && ...other);

/**
 * Prints a warn message by recursively output the variadic arguments.
 * @param first the first argument.
 * @param other the other arguments.
 */
template<typename FirstType, typename ...OtherTypes>
void warn_internal(FirstType && first, OtherTypes && ...other);

/**
 * End function for the recursive warn call.
 * Prints end of line and eventually flushes the output.
 */
void warn_internal();

// -----------------------------------------------------------------
// BENCHMARK -------------------------------------------------------
// -----------------------------------------------------------------

/**
 * Prints a benchmark message by recursively output the variadic arguments.
 * The message will be put on the output stream onlt if the benchmark mode is enabled.
 * @param logTag tag of the entity that is printing this message.
 * @param first the first argument.
 * @param other the other arguments.
 */
template<typename FirstType, typename ...OtherTypes>
void benchmark(const char *logTag, FirstType && first, OtherTypes && ...other);

/**
 * Prints a benchmark message by recursively output the variadic arguments.
 * @param first the first argument.
 * @param other the other arguments.
 */
template<typename FirstType, typename ...OtherTypes>
void benchmark_internal(FirstType && first, OtherTypes && ...other);

/**
 * End function for the recursive benchmark call.
 * Prints end of line and eventually flushes the output.
 */
void benchmark_internal();

#include "logger.tpp"

#endif // LOGGER_H
