#ifndef GEOMETRY_UTIL_H
#define GEOMETRY_UTIL_H

#include <iostream>

struct Point {
	double x;
	double y;
} typedef Point;

struct IPoint {
	int x;
	int y;
} typedef IPoint;

struct Line {
	double m;
	double q;
} typedef Line;

class GeometryUtil {
public:
	static Line lineForTwoPoints(const Point &p1, const Point &p2);
	static Point intersectionForTwoLines(const Line &l1, const Line &l2);
	static double distanceBetweenTwoPoints(const Point &p1, const Point &p2);
	static double angleBetweenTwoPoints(const Point &p1, const Point &p2);
	static Point rotatePoint(const Point &p, const Point &c, const double radians);
	static Point scalePoint(const Point &p, const double scaleFactor);
	static double hypot(const double d1, const double d2);

	static IPoint toIPoint(const Point &p);
};

std::ostream& operator << (std::ostream &o, const IPoint &p);
std::ostream& operator << (std::ostream &o, const Point &p);
std::ostream& operator << (std::ostream &o, const Line &l);

#endif // GEOMETRY_UTIL_H
