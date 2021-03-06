#include "track_base.h"
#include "navigation.h"

#include <iostream>
#include <cmath>

using namespace std;

/* Public Function Definitions */

track_base::track_base() : altitude(0.)
{
	std::cout << "Track_base constructed" << std::endl;
}

track_base::track_base(Point position_) : position(position_), altitude(0.)
{
	std::cout << "Track_base constructed" << std::endl;
}

track_base::track_base(Point position_, course_speed course_speed_, double altitude_) :
		position(position_), c_speed(course_speed_), altitude(altitude_)
{
	std::cout << "Track_base constructed" << std::endl;
}

track_base::~track_base()
{
	std::cout << "Track_base destructed" << std::endl;
}


// range and bearing of this track from a specified position
compass_position track_base::get_range_and_bearing_from (const Point& p) const
{
	compass_position result(p, position);
	return result;
}

// range and bearing of this track from a specified track
compass_position track_base::get_range_and_bearing_from (const track_base * track_ptr) const
{
	compass_position result(track_ptr->position, position);
	return result;
}

// range and bearing of closet point of approach from a specified track
// the other track is "ownship" normally, so this appears to be "backwards"
compass_position track_base::get_CPA_range_and_bearing_from(const track_base *track_ptr) const
{
	double time_until_CPA;
	compass_position result =
		compute_CPA(track_ptr->get_course_speed(), c_speed, get_range_and_bearing_from(track_ptr), time_until_CPA);
	return result;
}

// calculate whether this track is inbound towards a specified track
bool track_base::is_inbound_toward(const track_base * track_ptr) const
{
	bool result = false;
	// is this correct if course/bearing on different sides?
	// get reciprocal of bearing from this track
	compass_position other_position = get_range_and_bearing_from(track_ptr);
		
	// is our course within 90 degrees of reciprocal of other's bearing?
	double reciprocal_bearing = fmod(other_position.bearing + 180., 360.);
	
	double diff = fabs(c_speed.course - reciprocal_bearing);
	diff = (diff > 180) ? (360 - diff) : diff; // normalize to smallest angle
	if (diff < 90.)
		result = true;
	else
		result = false;
	return result;
}

// update the position of this object
void track_base::update_position(double time_increment)
{
	position = position + (c_speed * time_increment);
}

