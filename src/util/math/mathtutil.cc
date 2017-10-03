#include <cmath>
#include "mathutil.h"
#include "logger.h"

double MathUtil::changeRange(const Range &r1, const Range &r2, double r1Val) {
    return (r1Val - r1.low) / (r1.high - r1.low) * (r2.high - r2.low) + r2.low;
}

int MathUtil::changeRangeToInt(const Range &r1, const Range &r2, double r1Val) {
    return nearbyint(changeRange(r1, r2, r1Val));
}

bool MathUtil::isInRange(const Range &r, double val) {
    return (val >= r.low && val < r.high);
}

int MathUtil::rangeBelongingTo(const BoundRange ranges[], int rangeCount, double val) {
    for (int i = 0; i < rangeCount; i++) {
        if (val >= ranges[i].low && val < ranges[i].high)
            return ranges[i].associatedValue;
    }
    return 0;
}

double MathUtil::mod(double d1, double d2) {
    if (d2 < 0)
        return mod(d1, -d2);
    double res = fmod(d1, d2);
    if (res < 0)
        res += d2;
    return res;
}

double MathUtil::random(const Range &r) {
    return (r.low + (double)rand() * (r.high - r.low) / RAND_MAX);
}