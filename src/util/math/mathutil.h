#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "mathentities.h"

class MathUtil {
public:

    static double changeRange(const Range &r1, const Range &r2, double r1Val);
    static int changeRangeToInt(const Range &r1, const Range &r2, double r1Val);

    static bool isInRange(const Range &r, double d);

    static double mod(double d1, double d2);

    static double random(const Range &r);



    static double rangeBelongingTo(const BoundRange ranges[], int rangeCount, double val);
};

#endif // MATH_UTIL_H