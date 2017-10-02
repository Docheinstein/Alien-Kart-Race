#include <iostream>

#define DEBUG_OUTPUT_ENABLED 1
#define WARN_OUTPUT_ENABLED 1
#define BENCHMARK_OUTPUT_ENABLED 1

// Not defined here since dinamically defined in the Makefile.
/*
Uncomment if needed.
#define DEBUG_OUTPUT_LEVEL_MASK (\
(1 << 0)\
| (1 << 1) \
)
*/

// Whether flush the output stream after every print.
// This should be set to 1 carefully since really slows
// down the output process, but guarantee that debug message
// are sent to the output stream even in case of crashes.
#define FLUSH_ON_OUTPUT 0

#define BENCHMARK_TAG   "[BENCHMARK] -- "
#define DEBUG_TAG       "[DEBUG] ------ "
#define WARN_TAG        "[WARN] ------- "


// -----------------------------------------------------------------
// DEBUG -----------------------------------------------------------
// -----------------------------------------------------------------

inline
void debug_internal() {
	std::cout << std::endl;

	#if FLUSH_ON_OUTPUT
		std::cout << std::flush;
	#endif
}

template<typename FirstType, typename ...OtherTypes>
inline
void debug_internal(FirstType && first, OtherTypes && ...other) {
	std::cout << first;
	debug_internal(other...);
}

template<typename FirstType, typename ...OtherTypes>
inline
void debug( unsigned int level, const char *logTag,
            FirstType && first, OtherTypes && ...other) {
#if DEBUG_OUTPUT_ENABLED
	if (DEBUG_OUTPUT_LEVEL_MASK & level) {
			std::cout << DEBUG_TAG << logTag << first;
			debug_internal(other...);
	}
#endif
}


// -----------------------------------------------------------------
// WARN ------------------------------------------------------------
// -----------------------------------------------------------------

template<typename FirstType, typename ...OtherTypes>
inline
void warn(const char *logTag, FirstType && first, OtherTypes && ...other) {
#if WARN_OUTPUT_ENABLED
	std::cout << WARN_TAG << logTag << first;
	warn_internal(other...);
#endif
}

template<typename FirstType, typename ...OtherTypes>
inline
void warn_internal(FirstType && first, OtherTypes && ...other) {
	std::cout << first;
	warn_internal(other...);
}


inline
void warn_internal() {
	std::cout << std::endl;

	#if WARN_OUTPUT_ENABLED
		std::cout << std::flush;
	#endif
}


// -----------------------------------------------------------------
// BENCHMARK -------------------------------------------------------
// -----------------------------------------------------------------

template<typename FirstType, typename ...OtherTypes>
void benchmark(const char *logTag, FirstType && first, OtherTypes && ...other) {
#if BENCHMARK_OUTPUT_ENABLED
	std::cout << BENCHMARK_TAG << logTag << first;
	benchmark_internal(other...);
#endif
}

template<typename FirstType, typename ...OtherTypes>
void benchmark_internal(FirstType && first, OtherTypes && ...other) {
	std::cout << first;
	benchmark_internal(other...);
}


inline
void benchmark_internal() {
	std::cout << std::endl;

	#if FLUSH_ON_OUTPUT
		std::cout << std::flush;
	#endif
}



