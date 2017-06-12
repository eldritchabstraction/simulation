/***** Ship Class *****/
/* A ship has a name, initial position, amount of fuel, and parameters that govern its movement.
The initial amount of fuel is equal to the supplied fuel capacity - a full fuel tank.
A ship can be commanded to move to either a position, and island, or follow a course, or stop,
dock at or refuel at an island. It consumes fuel while moving, and becomes immobile
if it runs out of fuel. It inherits the Sim_object interface to the rest of the system,
and the track_base class provides the basic movement functionality, with the unit of time
corresponding to 1.0 for one "tick" - an hour of simulated time. The speeds and rates
are specified as per unit time, but in this project, the update time is always 1.0.

The update function updates the position and/or state of the ship.
The describe function outputs information about the ship state.
Accessors make the ship state available to either the public or to derived classes.
The is a "fat interface" for the capabilities of derived types of ships. These
functions are implemented in this class to throw an Error exception.
*/

/* 
This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You may define simple reader functions in the class declaration if you wish 
by changing the prototype to the definition. Your .h file for a component will always
be kept together with your .cpp file for the component.
You should delete this comment.
*/

#ifndef SHIP_H
#define SHIP_H

#include <iostream>

#include "Island.h"
#include "Sim_object.h"
#include "track_base.h"

using namespace std;

class Ship : public Sim_object
{
public:
	// initialize, then output constructor message
	Ship(const std::string& name, Point position, double fuel_capacity,
		double maximum_speed, double fuel_consumption, int resistance) : Sim_object(name) {}
		
	// made pure virtual to mark this class as abstract, but defined anyway
	// to output destructor message
	virtual ~Ship();
	
	/*** Readers ***/
	// return the current position
	// TODO: how does track_base hold this thing's position?
	// point get_location() const override {return track_base::get_position();}
	Point get_location() const override { return Point(); }
	
	// Return true if ship can move (it is not dead in the water or in the process or sinking); 
	bool can_move() const;
	
	// Return true if ship is moving
	bool is_moving() const;

	// Return true if ship is docked
	bool is_docked() const;
	
	// Return true if ship is afloat (not in process of sinking), false if not
	bool is_afloat() const;
		
	// Return true if ship is on the bottom
	bool is_on_the_bottom() const;
	
	// Return true if the ship is Stopped and the distance to the supplied island
	// is less than or equal to 0.1 nm
	bool can_dock(Island* island_ptr) const;
	
	/*** Interface to derived classes ***/
	// Update the state of the ship
	// TODO: implement
	void update() override {}
	// output a description of current state to cout
	// TODO: implement
	void describe() const override {}
	
	// TODO: implement
	void broadcast_current_state() const override {}
	
	/*** Command functions ***/
	// Start moving to a destination position at a speed
     // may throw Error("ship cannot move!")
     // may throw Error("ship cannot go that fast!")
	virtual void set_destination_position_and_speed(Point destination_position, double speed) {}
	// Start moving to a destination island at a speed
     // may throw Error("ship cannot move!")
     // may throw Error("ship cannot go that fast!")
	virtual void set_destination_island_and_speed(Island* destination_island, double speed) {}
	// Start moving on a course and speed
     // may throw Error("ship cannot move!")
     // may throw Error("ship cannot go that fast!");
	virtual void set_course_and_speed(double course, double speed) {}
	// Stop moving
     // may throw Error("ship cannot move!");
	virtual void stop() {}
	// dock at an island - set our position = island's position, go into Docked state
     // may throw Error("Can't dock!");
	virtual void dock(Island* island_ptr) {}
	// Refuel - must already be docked at an island; fill takes as much as possible
     // may throw Error("Must be docked!");
	virtual void refuel() {}

	/*** Fat interface command functions ***/
	// These functions throw an Error exception for this class
    // will always throw Error("Cannot load at a destination!");
	virtual void set_load_destination(Island*) {}
    // will always throw Error("Cannot unload at a destination!");
	virtual void set_unload_destination(Island*) {}
    // will always throw Error("Cannot attack!");
	virtual void attack(Ship* target_ptr_) {}
    // will always throw Error("Cannot attack!");
	virtual void stop_attack() {}

	// interactions with other objects
	// receive a hit from an attacker
	virtual void receive_hit(int hit_force, Ship* attacker_ptr) {}

protected:
	// future projects may need additional protected member functions

	double get_maximum_speed() const;
	// return pointer to the island currently docked at, or nullptr if not docked
	Island* get_docked_island() const;
	// return pointer to current destination island, nullptr if not set
	Island* get_destination_island() const;

private:
	double fuel_;						// Current amount of fuel
	double fuel_consumption_;			// tons/nm required
	Point destination_point_;			// Current destination position
	Island* destination_island_;		// Current destination island, if any
	
	// Updates position, fuel, and movement_state, assuming 1 time unit (1 hr)
	void calculate_movement();
};
#endif
