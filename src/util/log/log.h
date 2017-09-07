#ifndef LOG_H
#define LOG_H

#include <iostream>

#define FLUSH 1
#define DEBUG 1
inline
/*
void w() {
	std::cerr << std::endl;

	#if FLUSH
		std::cerr << std::flush;
	#endif
}
*/
void d() {
#if DEBUG
	std::cout << std::endl;

	#if FLUSH
		std::cout << std::flush;
	#endif
#endif
}
/*
void i() {
	std::cout << std::endl;

	#if FLUSH
		std::cout << std::flush;
	#endif
}
*/
template<typename FirstType, typename ...OtherTypes>
inline
void d(FirstType && first, OtherTypes && ...other) {
#if DEBUG
	std::cout << first;
	d(other...);
#endif
}
/*
template<typename FirstType, typename ...OtherTypes>
inline
void i(FirstType && first, OtherTypes && ...other) {
	std::cerr << first;
	i(other...);
}

template<typename FirstType, typename ...OtherTypes>
inline
void w(FirstType && first, OtherTypes && ...other) {
	std::cout << first;
	w(other...);
}
*/
#endif
