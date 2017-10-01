#ifndef GEOMETRY_UTIL_H
#define GEOMETRY_UTIL_H

#include <iostream>
#include "geometryentities.h"
#include "mathutil.h"

class GeometryUtil {
public:
	static Line lineForTwoPoints(const Point &p1, const Point &p2);
	static Point intersectionForTwoLines(const Line &l1, const Line &l2);
	static double distanceBetweenTwoPoints(const Point &p1, const Point &p2);
	static double hypot(const double d1, const double d2);

	static IPoint toIPoint(const Point &p);
};

#endif // GEOMETRY_UTIL_H
