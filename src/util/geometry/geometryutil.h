#ifndef GEOMETRY_UTIL_H
#define GEOMETRY_UTIL_H

struct Point {
	double x;
	double y;
} typedef Point;

struct Line {
	double m;
	double q;
} typedef Line;

class GeometryUtil {
public:
	static Line lineForTwoPoints(const Point &p1, const Point &p2);
	static Point intersectionForTwoLines(const Line &l1, const Line &l2);
	static Point rotatePoint(const Point &p, const Point &c, const double radians);
	static Point scalePoint(const Point &p, const double scaleFactor);
	static double hypot(const double d1, const double d2);
};

#endif // GEOMETRY_UTIL_H
