#ifndef LOG_H
#define LOG_H

#include <iostream>

#define DEBUG 0
#define BENCHMARK 1

#define FLUSH 1

extern const char * DEBUG_TAG;
extern const char * BENCHMARK_TAG;

inline
void d_internal() {
	std::cout << std::endl;

	#if FLUSH
		std::cout << std::flush;
	#endif
}

template<typename FirstType, typename ...OtherTypes>
inline
void d_internal(FirstType && first, OtherTypes && ...other) {
	std::cout << first;
	d_internal(other...);
}

template<typename FirstType, typename ...OtherTypes>
inline
void d(FirstType && first, OtherTypes && ...other) {
#if DEBUG
	std::cout << DEBUG_TAG << first;
	d_internal(other...);
#endif
}

inline
void benchmark_internal() {
	std::cout << std::endl;

	#if FLUSH
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
#if BENCHMARK
	std::cout << BENCHMARK_TAG << first;
	benchmark_internal(other...);
#endif
}

#endif
