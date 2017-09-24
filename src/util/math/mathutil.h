#ifndef MATH_UTIL_H
#define MATH_UTIL_H

class MathUtil {
public:
    struct Range {
        double low;
        double high;
    } typedef Range;

    static double changeRange(const Range &r1, const Range &r2, double r1Val);
    static int changeRangeToInt(const Range &r1, const Range &r2, double r1Val);

    static double mod(double d1, double d2);

    struct BoundRange {
        double low;
        double high;
        int associatedValue;
    } typedef BoundRange;

    static double rangeBelongingTo(const BoundRange ranges[], int rangeCount, double val);
};

#endif // MATH_UTIL_H