#ifndef MATH_ENTITIES_H
#define MATH_ENTITIES_H

struct Range {
    double low;
    double high;
} typedef Range;

struct BoundRange {
    double low;
    double high;
    int associatedValue;
} typedef BoundRange;

#endif // MATH_ENTITIES_H