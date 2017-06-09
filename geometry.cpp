/* 
Geometry.cpp implementation file
See Geometry.h for comments
*/

#include "geometry.h"

#include <iostream>
#include <cmath>

using namespace std;

// calculate a value for pi
const double pi = 2. * atan2(1., 0.);

// *** Member function definitions ***

// point members
// compare two positions
bool point::operator== (const point& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}
 
bool point::operator!= (const point& rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

// return the distance between two points
double cartesian_distance (const point& p1, const point& p2)
{
	double xd = p2.x - p1.x;
	double yd = p2.y - p1.y;
	double d = sqrt(xd * xd + yd * yd);
	return d;
}

// cartesian_vector members
// construct a cartesian_vector from two points,
// showing the vector from p1 to p2 
// that is, p1 + cv => p2
cartesian_vector::cartesian_vector(const point& p1, const point& p2)
{
	delta_x = p2.x - p1.x;
	delta_y = p2.y - p1.y;
}

// construct a cartesian_vector from a polar_vector
cartesian_vector::cartesian_vector(const polar_vector& pv)
{
	delta_x = pv.r * cos(pv.theta);
	delta_y = pv.r * sin(pv.theta);
}

// polar_vector members
// construct a polar_vector from a cartesian_vector
polar_vector::polar_vector(const cartesian_vector& cv)
{
	r = sqrt ((cv.delta_x * cv.delta_x) + (cv.delta_y * cv.delta_y));
	// atan2 will return neg angle for Quadrant III, IV, must translate to I, II
	theta = atan2 (cv.delta_y, cv.delta_x);
	if (theta < 0.)
		theta = 2. * pi + theta; // normalize theta positive
}

// construct a polar_vector from two points,
// showing the vector from p1 to p2 
// that is, p1 + pv => p2
polar_vector::polar_vector(const point& p1, const point& p2)
{
	polar_vector pv (cartesian_vector(p1, p2));
	r = pv.r;
	theta = pv.theta;
}




// *** Overloaded Operators ***

// Subtract two points to get a cartesian_vector
// p2's components are subtracted from p1
cartesian_vector operator- (const point& p1, const point& p2)
{
	return cartesian_vector(p1.x - p2.x, p1.y - p2.y);
}

// Add a point and a cartesian_vector to get the displaced point
point operator+ (const point& p, const cartesian_vector& cv)
{
	return point(p.x + cv.delta_x, p.y + cv.delta_y);
}

point operator+ (const cartesian_vector& cv, const point& p)
{
	return p + cv;
}
	
// Add a point and a polar_vector to get the displaced point
point operator+ (const point& p, const polar_vector& pv)
{
	cartesian_vector cv (pv);
	return cv + p;
}

point operator+ (const polar_vector& pv, const point& p)
{
	return p + pv;
}

// Adding or subtracting two cartesian_vectors adds or subtracts the components
cartesian_vector operator+ (const cartesian_vector& cv1, const cartesian_vector& cv2)
{
	return cartesian_vector(cv1.delta_x + cv2.delta_x, cv1.delta_y + cv2.delta_y);
}

cartesian_vector operator- (const cartesian_vector& cv1, const cartesian_vector& cv2)
{
	return cartesian_vector(cv1.delta_x - cv2.delta_x, cv1.delta_y - cv2.delta_y);
}

// divide a cartesian_vector by a double: divide each component by the double
cartesian_vector operator/ (const cartesian_vector& cv, double d)
{
	return cartesian_vector(cv.delta_x / d, cv.delta_y / d);
}

cartesian_vector operator/ (double d, const cartesian_vector& cv)
{
	return cv / d;
}

// divide a polar_vector by a double: divide r component by the double
polar_vector operator/ (const polar_vector& pv, double d)
{
	return polar_vector(pv.r / d, pv.theta);
}

polar_vector operator/ (double d, const polar_vector& pv)
{
	return pv / d;
}

// multiply a cartesian_vector by a double: multiply each component by the double
cartesian_vector operator* (const cartesian_vector& cv, double d)
{
	return cartesian_vector(cv.delta_x * d, cv.delta_y * d);
}

cartesian_vector operator* (double d, const cartesian_vector& cv)
{
	return cv * d;
}

// multiply a polar_vector by a double: multiply r component by the double
polar_vector operator* (const polar_vector& pv, double d)
{
	return polar_vector(pv.r * d, pv.theta);
}

polar_vector operator* (double d, const polar_vector& pv)
{
	return pv * d;
}

// Output operators
// output a point as "(x, y)"
ostream& operator<< (ostream& os, const point& p)
{
	os << '(' << p.x << ", " << p.y << ')';
	return os;
}
	
// output a cartesian_vector as "<x, y>"
ostream& operator<< (ostream& os, const cartesian_vector& cv)
{
	os << '<' << cv.delta_x << ", " << cv.delta_y << '>';
	return os;
}

// output a polar_vector as "P<r, theta>"
ostream& operator<< (ostream& os, const polar_vector& pv)
{
	os << "P<" << pv.r << ", " << pv.theta << '>';
	return os;
}

/***** Utility function definitions *****/
// There are 2pi radians in 360 degrees
double to_radians (double theta_d)
{
	return 2. * pi  * (theta_d / 360.);
}

double to_degrees (double theta_r)
{
	double temp = 360. * theta_r / (2. * pi);
	return temp;
}
