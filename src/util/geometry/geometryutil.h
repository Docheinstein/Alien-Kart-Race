#ifndef GEOMETRY_UTIL_H
#define GEOMETRY_UTIL_H

#include <iostream>
#include "geometryentities.h"
#include "mathutil.h"

/** Provides utility methods for geometry entities. */
class GeometryUtil {
public:
	/**
	 * Returns the line that link two points.
	 * @param  p1 a point.
	 * @param  p2 a point.
	 * @return    the line between two points.
	 */
	static Line lineForTwoPoints(const Point &p1, const Point &p2);

	/**
	 * Returns the unique point which is intersection of the given lines.
	 * @param  l1 a line.
	 * @param  l2 a line.
	 * @return    the intersection point.
	 */
	static Point intersectionForTwoLines(const Line &l1, const Line &l2);

	/**
	 * Returns the distance between two points (always > 0).
	 * @param  p1 a point.
	 * @param  p2 a point.
	 * @return    the distance between two points.
	 */
	static double distanceBetweenTwoPoints(const Point &p1, const Point &p2);

	/**
	 * Returns the hypotenuse of a triangle with the given edges.
	 * Actually sqrt(d1 * d1 + d2 *d2).
	 * @param  d1 an edge measure.
	 * @param  d2 an edge measure.
	 * @return    the hypotenus of the triangle.
	 */
	static double hypot(const double d1, const double d2);

	/**
	 * Returns the point approssimated to integer coordinates.
	 * @param  p the point.
	 * @return   the integer point.
	 */
	static IPoint toIPoint(const Point &p);
};

#endif // GEOMETRY_UTIL_H
