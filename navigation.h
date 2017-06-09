#ifndef NAVIGATION_H
#define NAVIGATION_H

/*
This set of simple classes is used to describe and compute positions, courses, and 
changes to positions and courses in normal Navigation domain expressions.
The classes are defined with "struct" to make all members public by default.

These classes and functions implement "plane sailing" in which movement is assumed 
to be on a plane, and so the rules of plane geometry and trigonometry apply. In practice,
this is quite accurate at reasonably small distances.  Large-scale navigation must take
the almost-spherical shape of the Earth into account, where position is represented in 
(latitude,longitude) coordinates in degrees measured North and South of the equator and 
East and West of the Prime Meridian. Computations on this scale require spherical 
trigonometry to be acceptably accurate, and much more complex computations are involved. 
For example, converting to/from (latitude,longitude) and points entails
taking the latitude into account to determining the difference in longitude and the 
point origin that is being assumed. In general, this entails assumptions about
the projection from  spherical earth coordinates to a plane, and thus does 
not have a general solution. The lat/lon classes such as Geo_position and the related functions 
for great_circle course calculations have been removed from this version of the Navigation module, 
leaving only the point class and functions for "plane sailing".

A point is represented in (x, y) coordinates in an nautical mile (nm) grid
whose origin is (0, 0). points can be compared with == and !=,
but since the values are doubles, equality should only be expected to result 
from assignment, not computation. 

Angles in Navigation are expressed in Compass degrees, where North is 0 degrees and East is 90 degrees,
South is 180 degrees, and West is 270 degrees. North is not 360 degrees, but is expressed as 0 degrees.
If North corresponds to "up" and East to "right" (as on standard map directions), 
then Navigation or Compass degrees to do not correspond to the normal geometry/trig representation 
in terms of quadrants where "up" is 90 degrees and "right" is zero degrees. This module includes 
functions for converting between the two systems of angle measurement.

A compass_position is represented as (bearing, range). This is normally relative to ownship.
It can be constructed from two points or a polar_vector. The angle
in compass_positions is in terms of "Compass" or "Navigation" degrees, where North is 0 degrees and
East is 90 degrees. 

A course_speed is (course, speed) - used to represent an object's course as normally
expressed as a direction in compass degrees and a speed in nm/hr. It is an "input" class only - 
no constructors are provided from other classes.
  
A compass_vector is (course, distance) - shows a displacement along a compass course for
a certain distance. 

To determine where a track is given its relation to ownship, add ownship's point and
the track's compass_position get the track's point:
	ownship_position + track_compass_position => track_position
	
To determine the displacement produced by following a course for some time,
multiply the course_speed by the time (a double) to get a compass_vector.
	track_course_speed * time => compass_vector
	time * track_course_speed => compass_vector

To determine where a track will be, add its current point and
the compass_vector for its course and the time to get its new point:
	current_track_position + track_compass_vector => new_track_position

*/

#include <iosfwd>

// forward declarations
struct point;
struct polar_vector;
struct course_speed;
struct compass_position;
struct compass_vector;

/* compass_position */
// compass_position describes a position in terms of bearing and range
struct compass_position
{
	double bearing;
	double range;	

	compass_position (double bearing_ = 0., double range_ = 0.) :
		bearing(bearing_), range(range_)
		{}

	// construct a compass_position from two points, giving
	// bearing and range of p2 from p1.
	compass_position(const point& p1, const point& p2);

	// construct a compass_position from a polar_vector
	compass_position (const polar_vector& pv);
};

/* course_speed */
// course_speed describes a compass course and speed.
// A course_speed can not be constructed from any other object.
struct course_speed
{
	double course;
	double speed;	

	course_speed (double course_ = 0., double speed_ = 0.) :
		course(course_), speed(speed_)
		{}
};


/* compass_vector */
// compass_vector describes a displacement in terms of compass direction and distance
struct compass_vector
{
	double direction;
	double distance;	

	compass_vector (double direction_ = 0., double distance_ = 0.) :
		direction(direction_), distance(distance_)
		{}

	compass_vector (const polar_vector& pv);

	// construct a compass_vector from two points, giving
	// the vector for moving from p1 to p2.
	compass_vector(const point& p1, const point& p2);

};

// *** Overloaded operator prototypes ***
// Operators are all defined as non-member functions for simplicity in documentation

// Adding a point and a compass_position yields a point
point operator+ (const point& p, const compass_position& cp);
point operator+ (const compass_position& cp, const point& p);

// Adding a point and a compass_vector yields a point
point operator+ (const point& p, const compass_vector& cv);
point operator+ (const compass_vector& cv, const point& p);

// Multiplying a course_speed by a double yields a compass_vector
compass_vector operator* (const course_speed& cs, double d);
compass_vector operator* (double d, const course_speed& cs);

// Output operator overloads
std::ostream& operator<< (std::ostream& os, const course_speed& p);
std::ostream& operator<< (std::ostream& os, const compass_position& cp);
std::ostream& operator<< (std::ostream& os, const compass_vector& cv);

// *** Other navigation functions  ***

// Given ownship's course and speed, and the target's course and speed, and bearing and range from ownship,
// compute the range and bearing of the point of closest approach and the time until the point.
// If the CPA is the current position, it is returned with the time being zero.
compass_position compute_CPA(course_speed ownship_cs, course_speed target_cs, compass_position target_position_cp, double& time_to_CPA);


#endif
