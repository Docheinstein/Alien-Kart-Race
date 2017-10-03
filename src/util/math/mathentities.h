#ifndef MATH_ENTITIES_H
#define MATH_ENTITIES_H

/** Range with a lower and an upper bound. */
struct Range {
    /** Lower bound. */
    double low;

    /** Upper bound. */
    double high;
} typedef Range;

/** Range with an associated value. */
struct BoundRange {
    /** Lower bound. */
    double low;

    /** Upper bound. */
    double high;

    /** Vallue associated with this range. */
    int associatedValue;
} typedef BoundRange;

#endif // MATH_ENTITIES_H