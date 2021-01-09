#include <cmath>

#include "Point.h"

#define PI 3.14159f

Point& Point::operator+=(const Point& p)
{
	X += p.X;
	Y += p.Y;

	return *this;
}

Point& Point::operator/=(const float f)
{
	X /= f;
	Y /= f;

	return *this;
}

Point Point::operator+(const Point& p) const
{
	return Point(X + p.X, Y + p.Y);
}

Point Point::operator-(const Point& p) const
{
	return Point(X - p.X, Y - p.Y);
}

Point Point::operator*(float f) const
{
	return Point(X * f, Y * f);
}

Point Point::operator/(float f) const
{
	return Point(X / f, Y / f);
}

bool Point::operator==(const Point& p) const
{
	return (X == p.X) && (Y == p.Y);
}

bool Point::operator!=(const Point& p) const
{
	return !(*this == p);
}

void Point::ChangeMagnitude(float new_magnitude)
{
	float magnitude = Magnitude(*this);
	magnitude /= new_magnitude;

	X /= magnitude;
	Y /= magnitude;
}

float Magnitude(const Vector& vector)
{
	return sqrtf(vector.X * vector.X + vector.Y * vector.Y);
}

float DotProduct(const Vector& v1, const Vector& v2)
{
	const float dist1 = Magnitude(v1);
	const float dist2 = Magnitude(v2);

	return ((v1.X * v2.X) + (v1.Y * v2.Y)) / (Magnitude(v1) * Magnitude(v2));
}

float ScalarTripleProduct(const Vector& v1, const Vector& v2)
{
	return v1.X * v2.Y - v2.X * v1.Y;
}

float GetAngleBetween(const Vector& v1, const Vector& v2)
{
	// acos gives smaller angle between two vectors.
	float angle = std::acosf(DotProduct(v1, v2)) / PI * 180.f;

	// Determine the direction of angle depends on the sign of vector that is computed by cross product of two vectors.
	if (ScalarTripleProduct(v1, v2) > 0)
		angle = 360.f - angle;

	return angle;
}

float DistanceBetween(const Point& p1, const Point& p2)
{
	const float dx = p1.X - p2.X;
	const float dy = p1.Y - p2.Y;

	return std::sqrtf(dx * dx + dy * dy);
}

float TriArea(const Point& p, const Point& p1, const Point& p2)
{
	// Cross product = area of parallelogram
	const Vector v1 = p1 - p;
	const Vector v2 = p2 - p;
	return v2.X * v1.Y - v1.X * v2.Y;
}