#ifndef STD_UTIL_H
#define STD_UTIL_H

class STDUtil {
public:
    template <typename PtrIter>
    static void deleteContainerPtr(PtrIter from, PtrIter to) {
        while (from != to) {
            delete *from;
            from++;
        }
    }
};

#endif // STD_UTIL_H