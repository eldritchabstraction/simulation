
/*
Define the destructor function even if it was declared as a pure virtual function.
This seems odd, because pure virtual functions are usually not defined in the class
that declares them. But this is often done as a way to make a class abstract, 
if there is no other virtual function that makes sense to mark as pure. Here we
are defining it just to get the destructor message output.
*/
#include <iomanip>

#include "Ship.h"
#include "Model.h"

#include "geometry.h"
#include "track_base.h"

extern Model *global_model;

int state_submerged = state_sinking | state_sunk | state_bottom;
int state_moving = state_move_position | state_move_island | state_move_course;

using namespace std;

Ship::~Ship()
{
	cout << "ship "  << get_name() << " destructed" << endl;
}

bool Ship::can_move() const
{
    return !(state_ & state_submerged & state_dead_water);
}

bool Ship::is_moving() const
{
    return state_ & state_moving;
}

bool Ship::is_docked() const
{
    return state_ & state_docked;
}

bool Ship::is_afloat() const
{
    return !(state_ & state_submerged);
}

bool Ship::is_on_the_bottom() const
{
    return state_ & state_bottom;
}


bool Ship::can_dock(Island* island_ptr) const
{
    // if distance between this and island_ptr is less than 0.1nm and this is stopped return true
    if ((state_ & state_stopped) && (cartesian_distance(get_location(), island_ptr->get_location()) < 0.1))
        return true;
    else
        return false;
}

void Ship::update()
{
    // ship is sinking
    if (state_ & state_submerged)
    {
        switch(state_)
        {
        case state_sinking:
            state_ = state_sunk;
            cout << "sunk";
            global_model->notify_gone(get_name());
            break;
        case state_sunk:
        case state_bottom:
            state_ = state_bottom;
            cout << "on the bottom";
            break;
        default:
            cout << "Error: unhandled state!" << state_ << endl;
        }
    }
    // ship is afloat
    else
    {
        if (resistance_ <= 0)
        {
            state_ = state_sinking;
            // TODO: set speed to 0
            cout << "sinking";
            return;
        }
        switch(state_)
        {
        case state_move_course:
        case state_move_island:
        case state_move_position:
            calculate_movement();
            cout << "now at";
            global_model->notify_location(get_name(), get_location());
            break;
        case state_stopped:
            cout << "stopped at " <<  get_location();
            break;
        case state_docked:
            cout << "docked at " << get_docked_island();
            break;
        case state_dead_water:
            cout << "dead in the water at " << get_location();
            break;
        default:
            cout << "Error: unhandled state!" << state_ << endl;
        }



    }
}

void Ship::describe() const
{
    cout << get_name() << " at " << get_location();
    if (state_ & state_submerged)
    {
        switch (state_)
        {
        case state_sinking:
            cout << " sinking "; break;
        case state_sunk:
            cout << " sunk "; break;
        case state_bottom:
            cout << "on the bottom"; break;
        default:
            cout << "Error: unhandled state!" << state_ << endl;
        }
        return;
    }

    cout << " fuel: " << fuel_ << " tons, resistance " << resistance_;
    switch (state_)
    {
    case state_move_position:
        cout << " Moving to " << destination_point_ << " on course..."; break;
    case state_move_island:
        cout << " Moving to " << get_destination_island()->get_location() << " on course..."; break;
    case state_move_course:
        cout << " Moving on course"; break;
    case state_docked:
        cout << " Docked at " << get_destination_island()->get_name(); break;
    case state_stopped:
        cout << " Stopped"; break;
    case state_dead_water:
        cout << " Dead in the water"; break;
    default:
        cout << "Error: unhandled state!" << state_ << endl;
    }
    cout << endl;
}

void Ship::broadcast_current_state() const
{
    global_model->notify_location(get_name(), get_location());
}

double Ship::get_maximum_speed() const
{

}

Island* Ship::get_docked_island() const
{
    if (!is_docked())
        return nullptr;
    else
        return destination_island_;
}

Island* Ship::get_destination_island() const
{
    return destination_island_;
}

void Ship::set_destination_position_and_speed(Point destination_position, double speed)
{
    if (state_ & state_submerged & state_dead_water)
        throw(runtime_error("ship cannot move!"));
    else if (speed > get_maximum_speed())
        throw(runtime_error("ship cannot go that fast!"));

    auto compass_vector = Compass_vector(get_location(), destination_position);
    set_speed(speed);
    set_course(compass_vector.direction);

    if (state_ & state_docked)
    {
        destination_island_ = nullptr;
    }
    state_ = state_move_position;

    cout << " will sail on " << get_course_speed() << " to " << destination_position;

    destination_point_ = destination_position;
}

void Ship::set_destination_island_and_speed(Island* destination_island, double speed)
{
    if (state_ & state_submerged & state_dead_water)
        throw(runtime_error("ship cannot move!"));
    else if (speed > get_maximum_speed())
        throw(runtime_error("ship cannot go that fast!"));

    auto compass_vector = Compass_vector(get_location(), destination_island->get_location());
    set_speed(speed);
    set_course(compass_vector.direction);

    state_ = state_move_island;

    cout << " will sail on " << get_course_speed() << " to " << destination_island->get_name();

    destination_island_ = destination_island;
}

void Ship::set_course_and_speed(double course, double speed)
{
    if (state_ & state_submerged & state_dead_water)
        throw(runtime_error("ship cannot move!"));
    else if (speed > get_maximum_speed())
        throw(runtime_error("ship cannot go that fast!"));

    set_speed(speed);
    set_course(course);

    state_ = state_move_course;

    cout << " will sail on " << get_course_speed();

    destination_island_ = nullptr;

}

void Ship::stop()
{
    if (state_ & state_submerged & state_dead_water)
        throw(runtime_error("ship cannot move!"));

    set_speed(0.);

    cout << " stopping at " << get_location();
}

void Ship::dock(Island* island_ptr)
{
    if (state_ != state_stopped || !can_dock(island_ptr))
        throw(runtime_error("Can't dock!"));

    set_position(island_ptr->get_location());
    global_model->notify_location(get_name(), get_position());

    cout << "Docked at " << island_ptr->get_name();
}

void Ship::refuel()
{
    if (!(state_ & state_docked))
        throw(runtime_error("Must be docked!"));

    double missing_fuel = max_fuel_ - fuel_;
    if (missing_fuel < 0.05)
    {
        fuel_ = max_fuel_;
        return;
    }

    fuel_ += destination_island_->provide_fuel(missing_fuel);

    cout << get_name() << " now has " << setprecision(3) << fuel_ << "tons of fuel";
}

void Ship::set_load_destination(Island*)
{
}

void Ship::set_unload_destination(Island*)
{
}

void Ship::attack(Ship* target_ptr_)
{
}

void Ship::stop_attack()
{
}

void Ship::receive_hit(int hit_force, Ship* attacker_ptr)
{
    resistance_ -= hit_force;

    cout << get_name() << " hit with " << hit_force << "resistance now " << resistance_ << endl;
}

/* Private Function Definitions */

/*
Calculate the new position of a ship based on how it is moving, its speed, and
fuel state. This function should be called only if the state is 
moving_to_position, moving_to_island, or moving_on_course.

Track_base has an update_position(double time) function that computes the new position
of an object after the specified time has elapsed. If the ship is going to move
for a full time unit (one hour), then it will get go the "full step" distance,
so update_position would be called with time = 1.0. If we can move less than that,
e.g. due to not enough fuel, update position  will be called with the corresponding
time less than 1.0.

For clarity in specifying the computation, this code assumes the specified private variable names, 
but you may change the variable names or enum class names, or state names if you wish (e.g. movement_state).
*/
void Ship::calculate_movement(void)
{
	// Compute values for how much we need to move, and how much we can, and how long we can,
	// given the fuel state, then decide what to do.
	double time = 1.0;	// "full step" time
	// get the distance to destination
	double destination_distance = cartesian_distance(get_location(), destination_point_);
	// get full step distance we can move on this time step
	double full_distance = track_base::get_speed() * time;
	// get fuel required for full step distance
	double full_fuel_required = full_distance * fuel_consumption_;	// tons = nm * tons/nm
    // how far and how long can we sail in this time period based on the fuel state?
    double distance_possible, time_possible;
    if (full_fuel_required <= fuel_)
    {
        distance_possible = full_distance;
        time_possible = time;
    }
    else
    {
        distance_possible = fuel_ / fuel_consumption_;	// nm = tons / tons/nm
        time_possible = (distance_possible / full_distance) * time;
    }

    // are we are moving to a destination, and is the destination within the distance possible?
    if ((state_ == state_move_position || state_ == state_move_island)
        && destination_distance <= distance_possible)
    {
        // yes, make our new position the destination
        set_position(destination_point_);
        // we travel the destination distance, using that much fuel
        double fuel_required = destination_distance * fuel_consumption_;
        fuel_ -= fuel_required;
        set_speed(0.);
        state_ = state_stopped;
    }
    else
    {
        // go as far as we can, stay in the same movement state
        // simply move for the amount of time possible
        update_position(time_possible);
        // have we used up our fuel?
        if (full_fuel_required >= fuel_)
        {
            fuel_ = 0.0;
            set_speed(0.);
            state_ = state_dead_water;
        }
        else
        {
            fuel_ -= full_fuel_required;
        }
    }
}
