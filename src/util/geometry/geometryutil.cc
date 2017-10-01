#include <cmath>
#include "geometryutil.h"

Line  GeometryUtil::lineForTwoPoints(const Point &p1, const Point &p2) {
  	Line l { 0, 0 };
	if (p2.x == p1.x)
		return l;
  	l.m = (p2.y - p1.y) / (p2.x - p1.x);
  	l.q = p1.y - l.m * p1.x;
  	return l;
}

Point GeometryUtil::intersectionForTwoLines(const Line &l1, const Line &l2) {
 	Point intersection { 0, 0 };
	if (l1.m == l2.m)
		return intersection;
  	intersection.x = (l2.q - l1.q) / (l1.m - l2.m);
  	intersection.y = l1.m * intersection.x + l1.q;
  	return intersection;
}

double GeometryUtil::distanceBetweenTwoPoints(const Point &p1, const Point &p2) {
	double xDiff = p2.x - p1.x;
	double yDiff = p2.y - p1.y;
	return hypot(xDiff, yDiff);
}


double GeometryUtil::hypot(double d1, double d2) {
	return sqrt(pow(d1, 2) + pow(d2, 2));
}

IPoint GeometryUtil::toIPoint(const Point &p) {
    return IPoint { (int) p.x, (int) p.y };
}
