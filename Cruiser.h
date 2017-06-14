/*
A Cruiser is a Ship with moderate speed, firepower, and resistance.
When told to attack it will start firing at the target if it is in range.
At each update, it will stop attacking if the target is either no longer afloat
(i.e. is sinking or sunk), or is out of range. As long as the target is both afloat
and in range, it will keep firing at it.

Initial values:
fuel capacity and initial amount: 1000, maximum speed 20., fuel consumption 10.tons/nm, 
resistance 6, firepower 3, maximum attacking range 15
*/

/* 
This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/

#ifndef CRUISER_H
#define CRUISER_H

#include <string>
#include "geometry.h"
#include "Ship.h"

enum cruiser_states
{
    state_attacking,
    state_not_attacking,
};

class Cruiser: public Ship
{

public:
	// initialize, then output constructor message
	Cruiser(const std::string& name, Point position);
	// output destructor message
	~Cruiser();

	// perform Cruiser-specific behavior
	void update() override;

	void describe() const override;

	// respond to an attack
    void receive_hit(int hit, Ship *attacker) override;

	// start an attack on a target ship
	void attack(Ship *target) override;

	void stop_attack() override;

private:
	cruiser_states cruiser_state_;

	double range_max_;
	int fire_power_;

	Ship *target_;
};

#endif
