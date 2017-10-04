#include <cmath>
#include "geometryentities.h"
#include "mathutil.h"

// ostream operators

std::ostream& operator << (std::ostream &o, const IPoint &ip) {
	o << "(x: " << ip.x << ", y: " << ip.y << ")";
	return o;
}

std::ostream& operator << (std::ostream &o, const Point &p) {
	o << "(x: " << p.x << ", y: " << p.y << ")";
	return o;
}

std::ostream& operator << (std::ostream &o, const Line &l) {
	o << "(m: " << l.m << ", q: " << l.q << ")";
	return o;
}

std::ostream& operator << (std::ostream &o, const Quad &q) {
	o 	<< "(UL: " << q.ul << "; UR: " << q.ur
		<< "; DR: " << q.dr << "; DL: " << q.dl << ")";
	return o;
}

std::ostream& operator << (std::ostream &o, const Vector &v) {
	o 	<< "(ang: " << v.direction << ", magnitude: " <<  v.magnitude << ")";
	return o;
}

std::ostream& operator << (std::ostream &o, const Angle &a) {
	o << a.rad << "rad";
	return o;
}

std::ostream& operator << (std::ostream &o, const DirectionalPoint &v) {
	o << "(pos: " << v.position << ", ang: " << v.direction << ")";
	return o;
}

// END ostream operators

// Quad

bool Quad::contains(const Point &p) const {
	return (
		p.y >= ul.y && p.x >= ul.x && // Check UL
		p.y >= ur.y && p.x <= ur.x && // Check UR
		p.y <= dl.y && p.x >= dl.x && // Check DL
		p.y <= dr.y && p.x <= dr.x    // Check DR
	);
}


bool Quad::intersects(const Quad &q) const {
	// Checks whether this quad contains at least a
	// point of the given quad, or vice-versa
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

// END Quad

// Angle

Angle::Angle(double angleRad) : rad(MathUtil::mod(angleRad, 2 * M_PI)) {
	// Always keep the angle between 0 and 2PI
}

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

// END Angle

// Vector

Vector & Vector::operator+=(const Vector &v) {
	// Add vector to vector with the head tail method
    Point headTailPointSum;
	headTailPointSum.x = sin(direction.rad) * magnitude;
	headTailPointSum.y = -cos(direction.rad) * magnitude;

	headTailPointSum.x += sin(v.direction.rad) * v.magnitude;
	headTailPointSum.y -= cos(v.direction.rad) * v.magnitude;

	DirectionalPoint dirPoint {Point {0, 0}, headTailPointSum};
	magnitude = hypot(headTailPointSum.x, headTailPointSum.y);
	direction = dirPoint.direction;
	return *this;
}

Vector operator +(Vector v1, const Vector &v2) {
    v1 += v2;
    return v1;
}

// END vector

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

// END DirectionalPoint