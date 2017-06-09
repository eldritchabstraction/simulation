#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iosfwd>

/*
This set of simple classes is used to compute positions and directions in the plane, 
in both cartesian and polar coordinates.

These classes are defined with "struct" to make all members public by default.

These classes make no assumptions about units of measurement of distance. Angles
can be specified in radians, but radians can be converted to and from 
trigonometry degrees in which 0 degrees corresponds to (x > 0, y = 0), 
and 90 degrees corresponds to (x = 0, y > 0).

point is a set of (x, y) coordinates.

A cartesian_vector is (delta_x, delta_y) - a displacement in Cartesian coordinates.

A polar_vector is (r, theta) - a displacement in polar coordinates using radians.

Various overloaded operators support computations of positions and directions.
*/

// angle units conversion functions
double to_radians(double theta_d);
double to_degrees(double theta_r);


// forward class declarations
struct cartesian_vector;
struct polar_vector;


/* point */
// A point contains an (x, y) pair to represent coordinates
struct point
{
	double x;
	double y;
	
	point (double x_ = 0., double y_ = 0.) :
		x(x_), y(y_)
		{}

	// compare two points
	bool operator== (const point& rhs) const;
	bool operator!= (const point& rhs) const;
};

// return the distance between two points
double cartesian_distance (const point& p1, const point& p2);

/* cartesian_vector */
// A cartesian_vector contains an x, y displacement
struct cartesian_vector
{
	double delta_x;
	double delta_y;

	cartesian_vector (double delta_x_ = 0., double delta_y_ = 0.) :
		delta_x(delta_x_), delta_y(delta_y_)
	{}

	// construct a cartesian_vector from two points,
	// showing the vector from p1 to p2 
	// that is, p1 + cv => p2
	cartesian_vector(const point& p1, const point& p2);

	// construct a cartesian_vector from a polar_vector
	cartesian_vector(const polar_vector& pv);
};


/* polar_vector */
// polar_vector describes a displacement in terms of polar coordinates
// with angle in radians
struct polar_vector
{
	double r;
	double theta;

	polar_vector (double r_ = 0., double theta_ = 0.) :
		r(r_), theta(theta_)
	{}

	// construct a polar_vector from two points,
	// showing the vector from p1 to p2 
	// that is, p1 + pv => p2
	polar_vector(const point& p1, const point& p2);

	// construct a polar_vector from a cartesian_vector
	polar_vector(const cartesian_vector& cv);

};

// *** Overloaded Operators ***

// Subtract two points to get a cartesian_vector
// p2's components are subtracted from p1
cartesian_vector operator- (const point& p1, const point& p2);

// Add a point and a cartesian_vector to get the displaced point
point operator+ (const point& p, const cartesian_vector& cv);
point operator+ (const cartesian_vector& cv, const point& p);
	
// Add a point and a polar_vector to get the displaced point
point operator+ (const point& p, const polar_vector& pv);
point operator+ (const polar_vector& pv, const point& p);

// Adding or subtracting two cartesian_vectors adds or subtracts the components
cartesian_vector operator+ (const cartesian_vector& cv1, const cartesian_vector& cv2);
cartesian_vector operator- (const cartesian_vector& cv1, const cartesian_vector& cv2);

// divide a cartesian_vector by a double: divide each component by the double
cartesian_vector operator/ (const cartesian_vector& cv, double d);
cartesian_vector operator/ (double d, const cartesian_vector& cv);

// divide a polar_vector by a double: divide r component by the double
polar_vector operator/ (const polar_vector& pv, double d);
polar_vector operator/ (double d, const polar_vector& pv);

// multiply a cartesian_vector by a double: divide each component by the double
cartesian_vector operator* (const cartesian_vector& cv, double d);
cartesian_vector operator* (double d, const cartesian_vector& cv);

// multiply a polar_vector by a double: divide r component by the double
polar_vector operator* (const polar_vector& pv, double d);
polar_vector operator* (double d, const polar_vector& pv);

// Output operators
std::ostream& operator<< (std::ostream& os, const point& p);
std::ostream& operator<< (std::ostream& os, const cartesian_vector& cv);
std::ostream& operator<< (std::ostream& os, const polar_vector& pv);

#endif
