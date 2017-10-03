#ifndef STD_UTIL_H
#define STD_UTIL_H

/** Provides utility function for std stuff. */
class STDUtil {
public:
    /**
     * Deallocates the given container from the specified iterators
     * assuming that its content its a simple pointer
     * (not array or other structures that need to be recursively deleted).
     * @param from the iterator from which the deletion will begin.
     * @param to   the iterator at which the deletion will finish.
     */
    template <typename PtrIter>
    static void deleteContainerPtr(PtrIter from, PtrIter to);
};

#include "stdutil.tpp"

#endif // STD_UTIL_H