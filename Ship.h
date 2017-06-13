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

#ifndef SHIP_H
#define SHIP_H

#include <iostream>

#include "Island.h"
#include "Sim_object.h"
#include "track_base.h"

using namespace std;

enum states
{
    state_move_position = 0b1,            // moving to position
    state_move_island   = 0b10,           // moving to island
    state_move_course   = 0b100,          // moving on course
    state_docked        = 0b1000,
    state_stopped       = 0b10000,
    state_dead_water    = 0b100000,       // dead in water
    state_sinking       = 0b1000000,
    state_sunk          = 0b10000000,
    state_bottom        = 0b100000000,
};

extern int state_moving;
extern int state_submerged;

class Ship : public Sim_object, private track_base
{
public:
	// initialize, then output constructor message
    // TODO: these defaults are extrapolated, we need clear definitions
	Ship(const std::string& name, Point position, double fuel_max = 1000,
	    double speed_max = 10, double fuel_rate = 10, int resistance = 6) : Sim_object(name), track_base(position)
	{
	    fuel_max_ = fuel_max;
	    fuel_ = fuel_max_;
	    fuel_rate_ = fuel_rate;

	    speed_max_ = speed_max;

	    destination_island_ = nullptr;
	    state_ = state_stopped;

	    resistance_ = resistance;

	    cout << "Ship " << name << " constructed\n";
	}

	// made pure virtual to mark this class as abstract, but defined anyway
	// to output destructor message
	virtual ~Ship();
	
	/*** Readers ***/
	// return the current position
	Point location() const override {return track_base::get_position();}
	
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
	void update() override;
	// output a description of current state to cout
	void describe() const override;
	
	void broadcast_current_state() const override;
	
	/*** Command functions ***/
	// Start moving to a destination position at a speed
     // may throw Error("ship cannot move!")
     // may throw Error("ship cannot go that fast!")
	virtual void set_destination_position_and_speed(Point destination_position, double speed);
	// Start moving to a destination island at a speed
     // may throw Error("ship cannot move!")
     // may throw Error("ship cannot go that fast!")
	virtual void set_destination_island_and_speed(Island* destination_island, double speed);
	// Start moving on a course and speed
     // may throw Error("ship cannot move!")
     // may throw Error("ship cannot go that fast!");
	virtual void set_course_and_speed(double course, double speed);
	// Stop moving
     // may throw Error("ship cannot move!");
	virtual void stop();
	// dock at an island - set our position = island's position, go into Docked state
     // may throw Error("Can't dock!");
	virtual void dock(Island* island_ptr);
	// Refuel - must already be docked at an island; fill takes as much as possible
     // may throw Error("Must be docked!");
	virtual void refuel();

	/*** Fat interface command functions ***/
	// These functions throw an Error exception for this class
    // will always throw Error("Cannot load at a destination!");
	virtual void set_load_destination(Island*);
    // will always throw Error("Cannot unload at a destination!");
	virtual void set_unload_destination(Island*);
    // will always throw Error("Cannot attack!");
	virtual void attack(Ship* target_ptr_);
    // will always throw Error("Cannot attack!");
	virtual void stop_attack();

	// interactions with other objects
	// receive a hit from an attacker
	virtual void receive_hit(int hit_force, Ship* attacker_ptr);

protected:
	// future projects may need additional protected member functions

	double get_fuel() const;
	double get_maximum_speed() const;
	// return pointer to the island currently docked at, or nullptr if not docked
	Island* get_docked_island() const;
	// return pointer to current destination island, nullptr if not set
	Island* get_destination_island() const;
	int state_;

private:
	double fuel_;						// Current amount of fuel
	double fuel_max_;
	double fuel_rate_;			// tons/nm required
	double speed_max_;

	Point destination_point_;			// Current destination position
	Island* destination_island_;		// Current destination island, if any
	int resistance_;                 // ship hitpoints, essentially
	
	// Updates position, fuel, and movement_state, assuming 1 time unit (1 hr)
	void calculate_movement(void);
};
#endif
