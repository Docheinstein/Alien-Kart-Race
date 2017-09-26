#ifndef GEOMETRY_UTIL_H
#define GEOMETRY_UTIL_H

#include <iostream>
#include "mathutil.h"

struct Point {
	double x;
	double y;
} typedef Point;

struct IPoint {
	int x;
	int y;
	operator Point() {
		return Point {static_cast<double>(x), static_cast<double>(y)};
	}
} typedef IPoint;

struct Line {
	// Line(const Vector &v);
	double m;
	double q;

} typedef Line;

// Angle

struct Angle {
	Angle() {}
	Angle(double rad);

    double rad;

	Angle & operator+=(const Angle &a);
    Angle & operator+=(double rad);
    Angle & operator-=(Angle a);
    Angle & operator-=(double rad);
} typedef Angle;

Angle operator +(Angle a1, const Angle &a2);
Angle operator +(Angle a1, double rad);
Angle operator -(Angle a1, const Angle &a2);
Angle operator -(Angle a1, double rad);

// End Angle

struct Vector {
	Vector() {}
	Vector(const Point &pStart, const Point &pEnd);
	Vector(const Point &p, const Angle &a);
	Point position;
	Angle direction;

	void advance(double length);

	Vector& operator+=(const Angle &a);
	Vector& operator+=(double rad);
	Vector& operator-=(Angle a);
	Vector& operator-=(double rad);
} typedef Vector;

Vector operator+(Vector v1, const Angle &angle);
Vector operator+(Vector v1, double angleRad);
Vector operator-(Vector v1, const Angle &angle);
Vector operator-(Vector v1, double angleRad);

Angle operator-(Vector v1, const Point &p);

class GeometryUtil {
public:
	static Line lineForTwoPoints(const Point &p1, const Point &p2);
	static Point intersectionForTwoLines(const Line &l1, const Line &l2);
	static double distanceBetweenTwoPoints(const Point &p1, const Point &p2);

	static double angleBetweenTwoPoints(const Point &p1, const Point &p2);
	static Angle angleDifference(const Vector &v1, const Point &p1);

	static Point rotatePoint(const Point &p, const Point &c, const double radians);
	static Point scalePoint(const Point &p, const double scaleFactor);
	static double hypot(const double d1, const double d2);

	static IPoint toIPoint(const Point &p);
};

std::ostream& operator << (std::ostream &o, const IPoint &ip);
std::ostream& operator << (std::ostream &o, const Point &p);
std::ostream& operator << (std::ostream &o, const Line &l);
std::ostream& operator << (std::ostream &o, const Angle &a);
std::ostream& operator << (std::ostream &o, const Vector &v);
#endif // GEOMETRY_UTIL_H
