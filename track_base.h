/*
The track_base class defines a base class of track objects, which are objects that move
according to course and speed. They have a point, a course_speed, and an altitude
(set to zero for surface tracks). When updated, they change their point
as a function of their course_speed.

Various values can be calculated for this track's position or motion as viewed from
some other track.
*/

#ifndef TRACK_BASE_H
#define TRACK_BASE_H

#include "geometry.h"
#include "navigation.h"

class track_base {
public:
	// Constructors
	track_base();
	track_base(Point position_);
	track_base(Point position_, course_speed course_speed_, double altitude_ = 0.);
	virtual ~track_base();
	
	// Readers
	Point get_position() const
		{return position;}
	course_speed get_course_speed() const
		{return c_speed;}
	double get_course() const 
		{return c_speed.course;}
	double get_speed() const 
		{return c_speed.speed;}
	double get_altitude() const
		{return altitude;}
			
	// Writers
	void set_position(Point position_)
		{position = position_;}
	void set_course_speed(const course_speed& course_speed_)
		{c_speed = course_speed_;}
	void set_course (double course_)
		{c_speed.course = course_;}
	void set_speed (double speed_)
		{c_speed.speed = speed_;}
	void set_altitude (double altitude_)
		{altitude = altitude_;}
		
	/* Calculate track motion analysis results from this track and a supplied 
	other track or position - the other track is normally "ownship", so
	these look "backwards" at first glance
	*/

	// range and bearing of this track from a specified position
	compass_position get_range_and_bearing_from(const Point & p) const;

	// range and bearing of this track from a specified other track  
	compass_position get_range_and_bearing_from(const track_base * track_ptr) const;

	// range and bearing of closet point of approach of this track from a specified track
	compass_position get_CPA_range_and_bearing_from(const track_base * track_ptr) const;
	
	// calculate whether this track is inbound towards a specified track
	bool is_inbound_toward(const track_base * track_ptr) const;
	
	// Update the position of this object using the supplied time increment
	// which is multiplied by the speed to get the distance to be moved
	virtual void update_position(double time_increment);
	
private:
	Point position;				// Current location
	course_speed c_speed;			// Current course & speed
	double altitude;					// Current altitude
};

#endif
