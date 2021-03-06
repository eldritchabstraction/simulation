/*
A Tanker is a ship with a large corgo capacity for fuel.
It can be told an Island to load fuel at, and an Island to unload at.
Once it is sent to the loading destination, it will start shuttling between 
the loading and unloading destination. At the loading destination, 
it will first refuel then wait until its cargo hold is full, then it will
go to the unloading destination.

Initial values:
fuel capacity and initial amount 100 tons, maximum speed 10., fuel consumption 2.tons/nm, 
resistance 0, cargo capacity 1000 tons, initial cargo is 0 tons.
*/

/* 
This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/
#ifndef TANKER_H
#define TANKER_H

#include <string>
#include "Ship.h"
#include "Island.h"
#include "geometry.h"

enum tanker_state
{
    state_no_cargo          = 0b1,
    state_unloading         = 0b10,
    state_moving_loading    = 0b100,
    state_loading           = 0b1000,
    state_moving_unloading  = 0b10000,
};

class Tanker : public Ship
{
public:
	// initialize, the output constructor message
	Tanker(const std::string& name, Point position);
	// output destructor message
	~Tanker();
	
	// This class overrides these Ship functions so that it can check if this Tanker has assigned cargo destinations.
	// if so, throw an Error("Tanker has cargo destinations!"); otherwise, simply call the Ship functions.
	void set_destination_position_and_speed(Point destination_point, double speed) override;
	void set_destination_island_and_speed(Island* destination_island, double speed) override;
	void set_course_and_speed(double course, double speed) override;

	// Set the loading and unloading Island destinations
	// if both cargo destination are already set, throw Error("Tanker has cargo destinations!").
	// if they are the same, leave at the set values, and throw Error("Load and unload cargo destinations are the same!")
	// if both destinations are now set, start the cargo cycle
	void set_load_destination(Island* island) override;
	void set_unload_destination(Island* island) override;
	
	// when told to stop, clear the cargo destinations and stop
	void stop() override;
	
	// perform Tanker-specific behavior
	void update() override;
	void describe() const override;

private:
	int tanker_state_;
	Island *load_destination_, *unload_destination_;
	double cargo_, cargo_max_;
};

#endif
