/*
Navigation.cpp implementation file.
See Navigation.h for comments
*/

#include "navigation.h"
#include "geometry.h"

#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;


// Compass degrees and trig degrees are backwards from each other
// and are offset by 90 degrees: 
// 0 trig = 90 compass, 0 compass = 90 trig
// 180 trig = 270 compass, 180 compass = 270 trig
// input and is assumed to be positive, and output will be positive
// this function should never produce an output of 360 instead of zero
/*
 45.00 -> 45.00
  0.00 -> 90.00
 90.00 ->  0.00
360.00 -> 90.00
*/
inline double to_other_degrees(double deg_in)
{	
	return fmod(360. + 90. - deg_in, 360.);
}


inline polar_vector to_polar_vector(compass_vector cv)
{
	return polar_vector(cv.distance, to_radians(to_other_degrees(cv.direction)));
}

inline polar_vector to_polar_vector(compass_position cp)
{
	return polar_vector(cp.range, to_radians(to_other_degrees(cp.bearing)));
}


// *** compass_position members ***
compass_position::compass_position (const polar_vector& pv)
{
	bearing = to_other_degrees(to_degrees(pv.theta));
	range = pv.r;
}

// construct a compass_position from two points, giving
// bearing and range of p2 from p1.
compass_position::compass_position(const point& p1, const point& p2)
{
	polar_vector pv (p1, p2);
	bearing = to_other_degrees(to_degrees(pv.theta));
	range = pv.r;
}
	

// *** compass_vector members ***


// construct a compass_vector from two positions, giving
// the vector for moving from p1 to p2.
compass_vector::compass_vector(const point& p1, const point& p2)
{
	polar_vector pv (p1, p2);
	direction = to_other_degrees(to_degrees(pv.theta));
	distance = pv.r;
}

compass_vector::compass_vector (const polar_vector& pv)
{
	direction = to_other_degrees(to_degrees(pv.theta));
	distance = pv.r;
}


// *** Overloaded operators ***
// Operators are all defined as non-member functions for simplicity in documentation

// Adding a point and a compass_position yields a point
point operator+ (const point& p, const compass_position& cp)
{
	point pn = point(p + to_polar_vector(cp));
	return pn;
}
	
point operator+ (const compass_position& cp, const point& p)
{
	return p + cp;
}

// Adding a point and a compass_vector yields a point
point operator+ (const point& p, const compass_vector& cv)
{
	point pn = point(p + to_polar_vector(cv));
	return pn;
}

point operator+ (const compass_vector& cv, const point& p)
{
	return p + cv;
}


// Multiplying a course_speed by a double yields a compass_vector
// with same angle but scaled distance
compass_vector operator* (const course_speed& cs, double d)
{
	return compass_vector(cs.course, cs.speed * d);
}

compass_vector operator* (double d, const course_speed& cs)
{
	return cs * d;
}

// Output operator overloads

// output a course_speed as "course deg, speed nm/hr"
ostream& operator<< (ostream& os, const course_speed& cs)
{
	// if course will round to 360.00 in the output (2 decimal places),
	// alter the output value to be 0.00; leave the actual direction alone.
	// The true value of direction is assumed to be always less than exactly 360.

	assert(cs.course < 360.);	// catch a programming error

	double output_course = cs.course;
	
	if ((output_course + .005) >= 360.)
		output_course = 0.00;
	
	os << "course " << output_course << " deg, speed " << cs.speed << " nm/hr";
	return os;
}

// output a compass_position as "bearing deg, range nm"
ostream& operator<< (ostream& os, const compass_position& cp)
{
	// if bearing will round to 360.00 in the output (2 decimal places),
	// alter the output value to be 0.00; leave the actual bearing alone.
	// The true value of bearing is assumed to be always less than exactly 360.

	assert(cp.bearing < 360.);	// catch a programming error

	double output_bearing = cp.bearing;
	
	if ((output_bearing + .005) >= 360.)
		output_bearing = 0.00;
	
	os << "bearing " << output_bearing << " deg, range " << cp.range << " nm";
	return os;
}

// output a compass_vector as "direction deg, distance nm"
ostream& operator<< (ostream& os, const compass_vector& cv)
{
	// if direction will round to 360.00 in the output (2 decimal places),
	// alter the output value to be 0.00; leave the actual direction alone.
	// The true value of direction is assumed to be always less than exactly 360.

	assert(cv.direction < 360.);	// catch a programming error

	double output_direction = cv.direction;
	
	if ((output_direction + .005) >= 360.)
		output_direction = 0.00;
	
	os << "direction " << output_direction << " deg, distance " << cv.distance << " nm";
	return os;
}

// *** Other navigation functions  ***

// *** compute_CPA ***
// Given ownship's course and speed, and the target's course and speed, and bearing and range from ownship,
// compute the range and bearing of the point of closest approach and the time until the point.
// If the CPA is the current position, it is returned with the time being zero.
// The algorithm used is based on code written by Al Gerheim.

compass_position compute_CPA(course_speed ownship_cs, course_speed target_cs, compass_position target_position_cp, double& time_to_CPA)
{
	
	// convert the two courses and speeds to Cartesian vectors
	cartesian_vector ownship_cv(polar_vector(ownship_cs.speed, to_radians(to_other_degrees (ownship_cs.course))));
	cartesian_vector target_cv(polar_vector(target_cs.speed, to_radians(to_other_degrees (target_cs.course))));
	// compute a vector for the position of the target right now (time = 0) relative to ownship.
	cartesian_vector relative_target_position(to_polar_vector(target_position_cp));
	
	// compute a vector that describes the target's motion relative to ownship,
	// which is effectively at (0, 0).
	cartesian_vector relative_target_motion = target_cv - ownship_cv;

	// compute parameter along relative motion line corresponding to closest point - note similarity to 
	// distance-from-line computations - this is the time of closest approach.
	// If relative distance is decreasing, this time will be negative.
	
	double t = 	(relative_target_motion.delta_x * relative_target_position.delta_x + 
				 relative_target_motion.delta_y * relative_target_position.delta_y) 
				/
				(relative_target_motion.delta_x * relative_target_motion.delta_x +
			 	 relative_target_motion.delta_y * relative_target_motion.delta_y);
	
	// if t is greater than 0, it means closest point was the initial point, at time = 0.
	if (t > 0.) {
		time_to_CPA = 0.;
		return target_position_cp;
		}
		
	else {
		time_to_CPA = fabs(t);
		// advance the relative motion vector to time t in the future
		cartesian_vector future_target_displacement = time_to_CPA * relative_target_motion;
		return compass_position(polar_vector(relative_target_position + future_target_displacement));
		}
}



