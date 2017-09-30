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

ostream& operator << (ostream &o, const Quad &q) {
	o 	<< "(UL: " << q.ul << "; UR: " << q.ur
		<< "; DR: " << q.dr << "; DL: " << q.dl << ")";
	return o;
}

ostream& operator << (ostream &o, const Vector &v) {
	o 	<< "(ang: " << v.direction << ", magnitude: " <<  v.magnitude << ")";
	return o;
}

ostream& operator << (ostream &o, const Angle &a) {
	o << a.rad << "rad";
	return o;
}

ostream& operator << (ostream &o, const DirectionalPoint &v) {
	o << "<pos: " << v.position << ", ang: " << v.direction << ">";
	return o;
}
// Line

// Line::Line(const DirectionalPoint &v) {
// 	q = v.position.y;
// }

// End line

// Quad

bool Quad::contains(const Point &p) const {
	return (
		p.y >= ul.y && p.x >= ul.x && // Check UL
		p.y >= ur.y && p.x <= ur.x && // Check UR
		p.y <= dl.y && p.x >= dl.x && // Check DL
		p.y <= dr.y && p.x <= dr.x
	);
}


bool Quad::intersects(const Quad &q) const {
	return (
		contains(q.ul) ||
		contains(q.ur) ||
		contains(q.dr) ||
		contains(q.dl) ||
		q.contains(ul) ||
		q.contains(ur) ||
		q.contains(dr) ||
		q.contains(dl)
	);
}

// End quad

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

Vector & Vector::operator+=(const Vector &v) {
    Point headTailPointSum;
	headTailPointSum.x = sin(direction.rad) * magnitude;
	headTailPointSum.y = -cos(direction.rad) * magnitude;

	headTailPointSum.x += sin(v.direction.rad) * v.magnitude;
	headTailPointSum.y -= cos(v.direction.rad) * v.magnitude;

	// std::cout << *this << endl;
	// std::cout << v << endl;
	// std::cout << "headTailPointSum" << headTailPointSum << endl;

	DirectionalPoint dirPoint {Point {0, 0}, headTailPointSum};
	magnitude = hypot(headTailPointSum.x, headTailPointSum.y);
	direction = dirPoint.direction;
}

Vector operator +(Vector v1, const Vector &v2) {
    v1 += v2;
    return v1;
}

// End vector

// DirectionalPoint

DirectionalPoint::DirectionalPoint(const Point &pStart, const Point &pEnd) {
	position = Point (pStart);
	double xDiff = pEnd.x - pStart.x;
	double yDiff = -pEnd.y - (-pStart.y);
	direction = Angle {atan2(xDiff, yDiff) };
}

DirectionalPoint::DirectionalPoint(const Point &p, const Angle &a) : position(p), direction(a) {}

DirectionalPoint& DirectionalPoint::operator+=(const Angle &a) {
	direction += a;
	return *this;
}

DirectionalPoint& DirectionalPoint::operator+=(double rad) {
	Angle a { rad };
	*this += a;
	return *this;
}

DirectionalPoint& DirectionalPoint::operator-=(Angle a) {
	a.rad = -a.rad;
	direction += a;
	return *this;
}

DirectionalPoint& DirectionalPoint::operator-=(double rad) {
	Angle a { -rad };
	*this += a;
	return *this;
}

DirectionalPoint operator+(DirectionalPoint v1, const Angle &angle) {
	v1 += angle;
	return v1;
}

DirectionalPoint operator+(DirectionalPoint v1, double angleRad) {
	v1 += angleRad;
	return v1;
}

DirectionalPoint operator-(DirectionalPoint v1, const Angle &angle) {
	v1 -= angle;
	return v1;
}

DirectionalPoint operator-(DirectionalPoint v1, double angleRad) {
	v1 -= angleRad;
	return v1;
}

Angle operator-(DirectionalPoint v1, const Point &p) {
	DirectionalPoint vectToPoint = DirectionalPoint {v1.position, p};
	return  vectToPoint.direction - v1.direction;
}

void DirectionalPoint::advance(double magnitude) {
	advance(direction.rad, magnitude);
}

void DirectionalPoint::advance(const Vector & v) {
	advance(v.direction.rad, v.magnitude);
}

void DirectionalPoint::advance(double ang, double magnitude) {
	position.y -= cos(ang) * magnitude;
	position.x += sin(ang) * magnitude;
}

// End DirectionalPoint



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
	return hypot(xDiff, yDiff);
}

// Between 0 and 2 M_PI
double GeometryUtil::angleBetweenTwoPoints(const Point &p1, const Point &p2) {
	double xDiff = p2.x - p1.x;
	double yDiff = -p2.y - (-p1.y);
	return fmod(atan2(xDiff, yDiff) + M_PI * 2, M_PI * 2);
}

Angle GeometryUtil::angleDifference(const DirectionalPoint &v, const Point &p) {
	DirectionalPoint vDiff = DirectionalPoint {v.position, p};
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
