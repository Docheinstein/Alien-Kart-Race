#ifndef GEOMETRY_ENTITIES_H
#define GEOMETRY_ENTITIES_H

#include <iostream>

// Contains the geometry entities used for different purposes (either logic or graphics).

/** Point with x and y coordinates. */
struct Point {
	/** X coordinate. */
	double x;

	/** Y coordinate. */
	double y;
} typedef Point;

/** Point with x and y coordinates as integers. */
struct IPoint {
	/** X coordinate. */
	int x;

	/** Y coordinate. */
	int y;
	operator Point() {
		return Point {static_cast<double>(x), static_cast<double>(y)};
	}
} typedef IPoint;

/** Straight line with a certain slope and the y-intercept. */
struct Line {
	/** Slope. */
	double m;

	/** Y-axis intercept. */
	double q;
} typedef Line;

/** Quad with four corners. */
struct Quad {
	/** Up-left corner. */
	Point ul;

	/** Up-right corner. */
	Point ur;

	/* Down-right corner. */
	Point dr;

	/** Down-left corner. */
	Point dl;

	/**
	 * Returns whether this quad contains the given point within its area.
	 * @param  p the point.
	 * @return   whether the point is inside this quad area.
	 */
	bool contains(const Point &p) const;

	/**
	 * Returns whether this quad intersects another quad.
	 * @param  q another quad.
	 * @return   whether this quad intersects another quad.
	 */
	bool intersects(const Quad &q) const;
} typedef Quad;

/**
 * Represents an angle expressed in radians.
 * Actually this is a wrapper for a double that
 * remains in the range of 0 and 2PI.
 */
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

/**
 * Vector identified by a direction and a magnitude.
 */
struct Vector {
	/** Angle of the vector. */
	Angle direction;

	/** Value of the vector's magnitude. */
	double magnitude;

	Vector& operator+=(const Vector &v);
} typedef Vector;

Vector operator+(Vector v1, const Vector &v2);

/**
 * Point which have a direction.
 * This actually doesn't exist as geometry entity,
 * but is useful for different uses when doing calc
 * with points and angles.
 */
struct DirectionalPoint {
	/** Creates a directional point. */
	DirectionalPoint() {}

	/**
	 * Creates a direction point by giving the start and the end point.
	 * @param pStart a point.
	 * @param pEnd   a point.
	 */
	DirectionalPoint(const Point &pStart, const Point &pEnd);

	/**
	 * Creates a directional point.
	 * @param p the point.
	 * @param a the angle.
	 */
	DirectionalPoint(const Point &p, const Angle &a);

	/** Point. */
	Point position;

	/** Direction. */
	Angle direction;

	/**
	 * Moves the point forward in the current direction for the given magnitude.
	 * @param magnitude the magnitude of the point movement.
	 */
	void advance(double magnitude);

	/**
	 * Moves the point in the direction of the vector, using its magnitude.
	 * @param v the movement vector.
	 */
	void advance(const Vector &v);

	/**
	 * Moves the point in the given direction with the given magnited.
	 * @param ang       the angle of the movement.
	 * @param magnitude the magnitude of the movement.
	 */
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

// OUTPUT OPERATORS

std::ostream& operator << (std::ostream &o, const IPoint &ip);
std::ostream& operator << (std::ostream &o, const Point &p);
std::ostream& operator << (std::ostream &o, const Line &l);
std::ostream& operator << (std::ostream &o, const Quad &q);
std::ostream& operator << (std::ostream &o, const Angle &a);
std::ostream& operator << (std::ostream &o, const Vector &v);
std::ostream& operator << (std::ostream &o, const DirectionalPoint &v);

#endif // GEOMETRY_ENTITIES_H
