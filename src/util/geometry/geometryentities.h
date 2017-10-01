#ifndef GEOMETRY_ENTITIES_H
#define GEOMETRY_ENTITIES_H

#include <iostream>

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
	double m;
	double q;
} typedef Line;

struct Quad {
	Point ul;
	Point ur;
	Point dr;
	Point dl;

	bool contains(const Point &p) const;
	bool intersects(const Quad &q) const;
} typedef Quad;

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

struct Vector {
	Angle direction;
	double magnitude;

	Vector& operator+=(const Vector &v);
} typedef Vector;
Vector operator+(Vector v1, const Vector &v2);

struct DirectionalPoint {
	DirectionalPoint() {}
	DirectionalPoint(const Point &pStart, const Point &pEnd);
	DirectionalPoint(const Point &p, const Angle &a);
	Point position;
	Angle direction;

	void advance(double magnitude);
	void advance(const Vector &v);
	void advance(double ang, double magnitude);

	DirectionalPoint& operator+=(const Angle &a);
	DirectionalPoint& operator+=(double rad);
	DirectionalPoint& operator-=(Angle a);
	DirectionalPoint& operator-=(double rad);
} typedef DirectionalPoint;

DirectionalPoint operator+(DirectionalPoint v1, const Angle &angle);
DirectionalPoint operator+(DirectionalPoint v1, double angleRad);
DirectionalPoint operator-(DirectionalPoint v1, const Angle &angle);
DirectionalPoint operator-(DirectionalPoint v1, double angleRad);

Angle operator-(DirectionalPoint v1, const Point &p);

std::ostream& operator << (std::ostream &o, const IPoint &ip);
std::ostream& operator << (std::ostream &o, const Point &p);
std::ostream& operator << (std::ostream &o, const Line &l);
std::ostream& operator << (std::ostream &o, const Quad &q);
std::ostream& operator << (std::ostream &o, const Angle &a);
std::ostream& operator << (std::ostream &o, const Vector &v);
std::ostream& operator << (std::ostream &o, const DirectionalPoint &v);

#endif // GEOMETRY_ENTITIES_H
