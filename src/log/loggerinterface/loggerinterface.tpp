template<typename FirstType, typename ...OtherTypes>
inline
void LoggerInterface::d_level(unsigned int level, FirstType && first, OtherTypes && ...other) {
    if (canLog())
        debug(level, logTag(), "\t", first, other...);
}

template<typename FirstType, typename ...OtherTypes>
inline
void LoggerInterface::d(FirstType && first, OtherTypes && ...other) {
    d_level(1, first, other...);
}

template<typename FirstType, typename ...OtherTypes>
inline
void LoggerInterface::d1(FirstType && first, OtherTypes && ...other) {
    d_level(1, first, other...);
}

template<typename FirstType, typename ...OtherTypes>
inline
void LoggerInterface::d2(FirstType && first, OtherTypes && ...other) {
    d_level(2, first, other...);
}

template<typename FirstType, typename ...OtherTypes>
inline
void LoggerInterface::w(FirstType && first, OtherTypes && ...other) {
    if (canLog())
        warn(logTag(), "\t", first, other...);
}

template<typename FirstType, typename ...OtherTypes>
inline
void LoggerInterface::bench(FirstType && first, OtherTypes && ...other) {
    if (canLog())
        benchmark(logTag(), "\t", first, other...);
}