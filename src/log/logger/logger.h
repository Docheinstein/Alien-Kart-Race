#ifndef LOG_H
#define LOG_H

#include <iostream>

#define DEBUG_OUTPUT_ENABLED 1
#define WARN_OUTPUT_ENABLED 1
#define BENCHMARK_OUTPUT_ENABLED 1

// Higher is more verbose
#define DEBUG_OUTPUT_LEVEL_MASK (\
(1 << 0)\
| (1 << 1) \
)

#define FLUSH_ON_OUTPUT 1

extern const char * DEBUG_TAG;
extern const char * WARN_TAG;
extern const char * BENCHMARK_TAG;

// Debug

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
void debug(unsigned int level, FirstType && first, OtherTypes && ...other) {
#if DEBUG_OUTPUT_ENABLED
	if (DEBUG_OUTPUT_LEVEL_MASK & level) {
			std::cout << DEBUG_TAG << first;
			debug_internal(other...);
	}
#endif
}

// Warn

inline
void warn_internal() {
	std::cout << std::endl;

	#if WARN_OUTPUT_ENABLED
		std::cout << std::flush;
	#endif
}

template<typename FirstType, typename ...OtherTypes>
inline
void warn_internal(FirstType && first, OtherTypes && ...other) {
	std::cout << first;
	warn_internal(other...);
}

template<typename FirstType, typename ...OtherTypes>
inline
void warn(FirstType && first, OtherTypes && ...other) {
#if WARN_OUTPUT_ENABLED
	std::cout << WARN_TAG << first;
	warn_internal(other...);
#endif
}

// Benchmark

inline
void benchmark_internal() {
	std::cout << std::endl;

	#if FLUSH_ON_OUTPUT
		std::cout << std::flush;
	#endif
}

template<typename FirstType, typename ...OtherTypes>
inline
void benchmark_internal(FirstType && first, OtherTypes && ...other) {
	std::cout << first;
	benchmark_internal(other...);
}

template<typename FirstType, typename ...OtherTypes>
inline
void benchmark(FirstType && first, OtherTypes && ...other) {
#if BENCHMARK_OUTPUT_ENABLED
	std::cout << BENCHMARK_TAG << first;
	benchmark_internal(other...);
#endif
}

#endif
