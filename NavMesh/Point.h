/*!*******************************************************************
\headerfile   Point.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for Point struct which defines point and
			  some functions for vector and general math.
********************************************************************/
#ifndef POINT_H
#define POINT_H

/*!*******************************************************************
\struct Point
\brief
	   Holding two float which are x and y coordinate value of point.
	   There are some overloaded operators helper method too.
********************************************************************/
struct Point
{
	/*!*******************************************************************
	\brief
		   Constructor that initializes position of point.

	\param x
		   X value. Default is 0.

	\param y
		   Y value. Default is 0.
	********************************************************************/
	Point(const float x = 0.f, const float y = 0.f)
		: X(x), Y(y) {}

	float X; //!< X-coordinate value.
	float Y; //!< Y-coordinate value.

	/*!*******************************************************************
	\brief
		   Overloaded operator +=. Adds both x and y to this point.

	\param p
		   Point (mathematically, it is a vector) to add.

	\return Point&
			This point (or maybe vector).
	********************************************************************/
	Point& operator+=(const Point& p);
	/*!*******************************************************************
	\brief
		   Overloaded operator /=. Divides both x and y by given float.

	\param f
		   Value to divide.

	\return Point&
			This point (mathematically, it is a vector).
	********************************************************************/
	Point& operator/=(const float f);

	/*!*******************************************************************
	\brief
		   Overloaded operator +. Returns sum of both x and y values.

	\param p
		   Point (mathematically, it is a vector) to add.

	\return Point
			New point (or maybe vector).
	********************************************************************/
	Point operator+(const Point& p) const;
	/*!*******************************************************************
	\brief
		   Overloaded operator -. Substract x and y from p to this point.

	\param p
		   Point (mathematically, it is a vector) to substract.

	\return Point
			New point (or maybe vector).
	********************************************************************/
	Point operator-(const Point& p) const;
	/*!*******************************************************************
	\brief
		   Overloaded operator *. Multiply given float to x and y.

	\param f
		   Value to multiply.

	\return Point
			New point (mathematically, it is a vector).
	********************************************************************/
	Point operator*(float f) const;
	/*!*******************************************************************
	\brief
		   Overloaded operator /. Divides given float to x and y.

	\param f
		   Value to divide.

	\return Point
			New point (mathematically, it is a vector).
	********************************************************************/
	Point operator/(float f) const;

	/*!*******************************************************************
	\brief
		   Overloaded operator ==. Compare both x and y.

	\param p
		   Point to compare.

	\return bool
			Return true, if both are same.
	********************************************************************/
	bool operator==(const Point& p) const;
	/*!*******************************************************************
	\brief
		   Overloaded operator !=. Compare both x and y.

	\param p
		   Point to compare.

	\return bool
			Return true, if both are different.
	********************************************************************/
	bool operator!=(const Point& p) const;

	/*!*******************************************************************
	\brief
		   Helper method to change magnitude of this vector.

	\param new_magnitude
		   New magnitude of this vector.
	********************************************************************/
	void ChangeMagnitude(float new_magnitude);
};

using Vector = Point;

const Point O(0.f, 0.f);

/*!*******************************************************************
\brief
		Helper method to compute magnitude of given vector.

\param vector
	   Vector to compute magnitude.

\return float
	    Magnitude of given vector.
********************************************************************/
float Magnitude(const Vector& vector);
/*!*******************************************************************
\brief
		Helper method to do dot product of given two vectors.

\param v1
	   One of vectors to do dot product.

\param v2
	   One of vectors to do dot product.

\return float
		Dot prodcut of two vectors.
********************************************************************/
float DotProduct(const Vector& v1, const Vector& v2);
/*!*******************************************************************
\brief
		Helper method to compute z-coordiante value of vector that is 
		result of doing cross product on given two 2D vectors (means 
		z-coordinate value is 0).

\param v1
	   One of vectors to do cross product.

\param v2
	   One of vectors to do cross product.

\return float
		Z-coordinate value of vector compted by doing cross product 
		on given two vectors.
********************************************************************/
float ScalarTripleProduct(const Vector& v1, const Vector& v2);
/*!*******************************************************************
\brief
		Helper method to compute rotating angle between two vectors.

\param v1
	   One of vectors to compute angle.

\param v2
	   One of vectors to compute angle.

\return float
		The angle between two vector. Range is [0, 360].
********************************************************************/
float GetAngleBetween(const Vector& v1, const Vector& v2);
/*!*******************************************************************
\brief
		Helper method to compute distance between given two points.

\param p1
	   One of points to compute distance.

\param p2
	   One of points to compute distance.

\return float
		The distance between two points.
********************************************************************/
float DistanceBetween(const Point& p1, const Point& p2);

/*!*******************************************************************
\brief
		Helper method to compute the area of parallelogram can be made
		by given three points.

\param p
	   Start vertex of two vectors.

\param p1
	   End vertex of one of vetors.

\param p2
	   End vertex of one of vetors.

\return float
		The area of paralleogram.
********************************************************************/
float TriArea(const Point& p, const Point& p1, const Point& p2);

#endif // !POINT_H
