#include <cmath>
#include "geometryutil.h"
#include "logger.h"

using namespace std;

ostream& operator << (ostream &o, const IPoint &ip) {
	o << "(x: " << ip.x << ", y: " << ip.y << ")";
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

ostream& operator << (ostream &o, const Angle &a) {
	o << a.rad << "rad";
	return o;
}

ostream& operator << (ostream &o, const Vector &v) {
	o << "<pos: " << v.position << ", " << "ang: " << v.direction << ">";
	return o;
}
// Line

// Line::Line(const Vector &v) {
// 	q = v.position.y;
// }

// End line

// Angle

Angle::Angle(double angleRad) : rad(MathUtil::mod(angleRad, 2 * M_PI)) {}


Angle & Angle::operator+=(const Angle &a) {
    rad = MathUtil::mod(rad + a.rad, 2 * M_PI);
    return *this;
}

Angle & Angle::operator+=(double rad) {
    Angle a = { rad };
    *this += a;
    return *this;
}

Angle & Angle::operator-=(Angle a) {
    a.rad = -a.rad;
    *this += a;
    return *this;
}

Angle & Angle::operator-=(double rad) {
	Angle a = { -rad };
    *this += a;
    return *this;
}


Angle operator +(Angle a1, const Angle &a2) {
    a1 += a2;
    return a1;
}

Angle operator +(Angle a1, double rad) {
    a1 += rad;
    return a1;
}

Angle operator -(Angle a1, const Angle &a2) {
    a1 -= a2;
    return a1;
}

Angle operator -(Angle a1, double rad) {
    a1 -= rad;
    return a1;
}

// End Angle

// Vector

Vector::Vector(const Point &pStart, const Point &pEnd) {
	position = Point (pStart);
	double xDiff = pEnd.x - pStart.x;
	double yDiff = -pEnd.y - (-pStart.y);
	direction = Angle {atan2(xDiff, yDiff) };
}

Vector::Vector(const Point &p, const Angle &a) : position(p), direction(a) {}

Vector& Vector::operator+=(const Angle &a) {
	direction += a;
	return *this;
}

Vector& Vector::operator+=(double rad) {
	Angle a { rad };
	*this += a;
	return *this;
}

Vector& Vector::operator-=(Angle a) {
	a.rad = -a.rad;
	direction += a;
	return *this;
}

Vector& Vector::operator-=(double rad) {
	Angle a { -rad };
	*this += a;
	return *this;
}

Vector operator+(Vector v1, const Angle &angle) {
	v1 += angle;
	return v1;
}

Vector operator+(Vector v1, double angleRad) {
	v1 += angleRad;
	return v1;
}

Vector operator-(Vector v1, const Angle &angle) {
	v1 -= angle;
	return v1;
}

Vector operator-(Vector v1, double angleRad) {
	v1 -= angleRad;
	return v1;
}

void Vector::advance(double length) {
	position.y -= cos(direction.rad) * length;
	position.x += sin(direction.rad) * length;
}

// End vector



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

double GeometryUtil::distanceBetweenTwoPoints(const Point &p1, const Point &p2) {
	double xDiff = p2.x - p1.x;
	double yDiff = p2.y - p1.y;
	return sqrt(xDiff * xDiff + yDiff * yDiff);
}

// Between 0 and 2 M_PI
double GeometryUtil::angleBetweenTwoPoints(const Point &p1, const Point &p2) {
	double xDiff = p2.x - p1.x;
	double yDiff = -p2.y - (-p1.y);
	return fmod(atan2(xDiff, yDiff) + M_PI * 2, M_PI * 2);
}

Angle GeometryUtil::angleDifference(const Vector &v, const Point &p) {
	Vector vDiff = Vector {v.position, p};
	debug(1,"\n\t v: ", v,
			"\n\t p; ", p,
			"\n\t vDiff: ", vDiff);
	return vDiff.direction - v.direction;
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
