#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "mathentities.h"

/** Provides utility methods for math (and logic). */
class MathUtil {
public:

    /**
     * Changes the range of a given value from the range 1 to the range 2.
     * @param  r1    the range that contains the given value.
     * @param  r2    the new range in which the value will be expressed.
     * @param  r1Val the value of the first range.
     * @return       the new value that the given value have within the second range.
     */
    static double changeRange(const Range &r1, const Range &r2, double r1Val);

    /**
     * Changes the range of a given value from the range 1 to the range 2,
     * approssimating to the nearest integer value.
     * @param  r1    the range that contains the given value.
     * @param  r2    the new range in which the value will be expressed.
     * @param  r1Val the value of the first range.
     * @return       the new value that the given value have within the second range.
     */
    static int changeRangeToInt(const Range &r1, const Range &r2, double r1Val);

    /**
     * Returns whether the given value is withing the given range.
     * @param  r the range.
     * @param  d the value.
     * @return   whether the given value is withing the given range.
     */
    static bool isInRange(const Range &r, double d);

    /**
     * Returns the modulo of d1 % d2.
     * This is differente from the default % operator since this
     * operates on either decimal or negative numbers.
     * @param  d1 the first operand of the modulo.
     * @param  d2 the first operand of the modulo.
     * @return    the modulo result.
     */
    static double mod(double d1, double d2);

    /**
     * Returns a random range within the given range.
     * @param  r the range.
     * @return   the random decimal number within the range.
     */
    static double random(const Range &r);

    /**
     * Returns the value associated with the range the contains the given value.
     * @param  ranges     an array of range with an associated value.
     * @param  rangeCount the count of the range in the array.
     * @param  val        the value.
     * @return            the value associated with the range the contains the given value.
     */
    static int rangeBelongingTo(const BoundRange ranges[], int rangeCount, double val);
};

#endif // MATH_UTIL_H