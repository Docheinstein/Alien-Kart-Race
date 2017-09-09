#include <cmath>
#include "geometryutil.h"
#include "log.h"

using namespace std;

ostream& operator << (ostream &o, const IPoint &p) {
	o << "(x: " << p.x << ", y: " << p.y << ")";
	return o;
}

ostream& operator << (ostream &o, const Point &p) {
	o << "(x: " << p.x << ", y: " << p.y << ")";
	return o;
}

ostream& operator << (ostream &o, const Line &l) {
	o << "(m: " << l.m << ", q: " << l.q << ")";
	return o;
}

Line  GeometryUtil::lineForTwoPoints(const Point &p1, const Point &p2) {
  	Line l { 0, 0 };
	if (p2.x == p1.x)
		return l;
  	l.m = (p2.y - p1.y) / (p2.x - p1.x);
  	l.q = p1.y - l.m * p1.x;
	//Line for points (",p1.x,",",p1.y,") and  (",p2.x,",",p2.y, ") is Y=",l.m,"x + ",l.q);
  	return l;
}

Point GeometryUtil::intersectionForTwoLines(const Line &l1, const Line &l2) {
 	Point intersection { 0, 0 };
	if (l1.m == l2.m)
		return intersection;
  	intersection.x = (l2.q - l1.q) / (l1.m - l2.m);
  	intersection.y = l1.m * intersection.x + l1.q;
/*
	Intersection of line y=", l1.m, "x + ", l1.q,
		" and line y=", l2.m, "x + ", l2.q, "\n",
		"on point: (", intersection.x, ", ", intersection.y, ")");
*/
  	return intersection;
}

Point GeometryUtil::rotatePoint(const Point &p, const Point &c, const double radians) {
	double xDiff = - c.x + p.x;
	double yDiff = - c.y + p.y;
	Point rotated {
		xDiff * cos(-radians) - yDiff * sin(-radians) + c.x,
		xDiff * sin(-radians) + yDiff * cos(-radians) + c.y
	};
	return rotated;
}

Point GeometryUtil::scalePoint(const Point &p, const double scaleFactor) {
	return Point { scaleFactor * p.x, scaleFactor * p.y };
}

double GeometryUtil::hypot(double d1, double d2) {
	return sqrt(pow(d1, 2) + pow(d2, 2));
}

IPoint GeometryUtil::toIPoint(const Point &p) {
    return IPoint { (int) p.x, (int) p.y };
}
