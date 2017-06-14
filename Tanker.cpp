
#include "Tanker.h"
#include "Ship.h"

const double c_tanker_fuel_max = 100;
const int c_tanker_resistance = 0;
const double c_tanker_speed_max = 10;
const double c_tanker_fuel_rate = 2;

using namespace std;

Tanker::Tanker(const std::string& name, Point position):
    Ship(name, position, c_tanker_fuel_max, c_tanker_speed_max, c_tanker_fuel_rate, c_tanker_resistance)
{
    tanker_state_ = state_no_cargo;
    cargo_ = 0;
    cargo_max_ = 1000;
    cout << "Tanker " << name << " constructed\n";
    unload_destination_ = load_destination_ = nullptr;
}

Tanker::~Tanker()
{
    cout << "Tanker " << name() << " destructed\n";
}

void Tanker::set_destination_position_and_speed(Point destination_point, double speed)
{
    if (tanker_state_ & state_no_cargo)
        throw(runtime_error("Tanker has no cargo destinations!"));

    Ship::set_destination_position_and_speed(destination_point, speed);
}

void Tanker::set_destination_island_and_speed(Island* destination_island, double speed)
{
    if (!(tanker_state_ & state_no_cargo))
        throw(runtime_error("Tanker has cargo destinations!"));
    Ship::set_destination_island_and_speed(destination_island, speed);
}

void Tanker::set_course_and_speed(double course, double speed)
{
    if (tanker_state_ & state_no_cargo)
        throw(runtime_error("Tanker has no cargo destinations!"));

    Ship::set_course_and_speed(course, speed);
}

void Tanker::set_load_destination(Island* island)
{
    if (state_ & state_no_cargo)
        throw(runtime_error("Tanker has no cargo destinations!"));

    load_destination_ = island;

    if (unload_destination_ == load_destination_)
        throw(runtime_error("Load and unload cargo destinations are the same!"));


    cout << name() << " will load at " << island->name() << endl;

    if (!unload_destination_ || !load_destination_)
        return;

    if (state_ == state_docked)
    {
        if (location() == load_destination_->location())
            tanker_state_ = state_loading;
        else if (location() == unload_destination_->location())
            tanker_state_ = state_unloading;
    }
    // ship is not moving
    else if (state_ & state_moving == 0)
    {
        if ((cargo_ == 0) && can_dock(load_destination_))
            tanker_state_ = state_loading;
        else if ((cargo_ > 0) && can_dock(unload_destination_))
            tanker_state_ = state_unloading;
    }
    else if (cargo_ == 0)
    {
        Ship::set_destination_island_and_speed(load_destination_, get_maximum_speed());
        tanker_state_ = state_moving_loading;
    }
    else if (cargo_ > 0)
    {
        Ship::set_destination_island_and_speed(unload_destination_, get_maximum_speed());
        tanker_state_ = state_moving_unloading;
    }
}

void Tanker::set_unload_destination(Island* island)
{
    if (state_ & state_no_cargo)
        throw(runtime_error("Tanker has no cargo destinations!"));

    unload_destination_ = island;

    if (unload_destination_ == load_destination_)
        throw(runtime_error("Load and unload cargo destinations are the same!"));


    cout << name() << " will unload at " << island->name() << endl;

    if (!unload_destination_ || !load_destination_)
        return;

    if (state_ == state_docked)
    {
        if (location() == load_destination_->location())
            tanker_state_ = state_loading;
        else if (location() == unload_destination_->location())
            tanker_state_ = state_unloading;
    }
    // ship is not moving
    else if (state_ & state_moving == 0)
    {
        if ((cargo_ == 0) && can_dock(load_destination_))
            tanker_state_ = state_loading;
        else if ((cargo_ > 0) && can_dock(unload_destination_))
            tanker_state_ = state_unloading;
    }
    else if (cargo_ == 0)
    {
        Ship::set_destination_island_and_speed(load_destination_, get_maximum_speed());
        tanker_state_ = state_moving_loading;
    }
    else if (cargo_ > 0)
    {
        Ship::set_destination_island_and_speed(unload_destination_, get_maximum_speed());
        tanker_state_ = state_moving_unloading;
    }
}

void Tanker::stop()
{
    load_destination_ = nullptr;
    unload_destination_ = nullptr;

    Ship::stop();

    tanker_state_ = state_no_cargo;

    cout << name() << " now has no cargo destinations\n";
}

void Tanker::update()
{
    Ship::update();

    if (state_ & state_submerged & state_dead_water)
    {
        tanker_state_ = state_no_cargo;
        load_destination_ = unload_destination_ = nullptr;
        cout << name() << " now has no cargo destinations/n";
    }
    else if (tanker_state_ == state_no_cargo)
    {

    }
    else if (tanker_state_ == state_moving_loading)
    {
        if ((state_ != state_moving) && (can_dock(get_destination_island())))
        {
            dock(get_destination_island());
            tanker_state_ = state_loading;
        }
    }
    else if (tanker_state_ == state_moving_unloading)
    {
        if ((state_ != state_moving) && (can_dock(get_destination_island())))
        {
            dock(get_destination_island());
            tanker_state_ = state_unloading;

        }
    }
    else if (tanker_state_ == state_loading)
    {
        Ship::refuel();

        double missing_cargo = cargo_max_ - cargo_;

        if (missing_cargo <= 0.05)
        {
            cargo_ = cargo_max_;
            Ship::set_destination_island_and_speed(unload_destination_, get_maximum_speed());
            tanker_state_ = state_moving_unloading;
        }
        else
        {
            cargo_ += get_destination_island()->provide_fuel(missing_cargo);
            cout << name() << " now has " << cargo_ << " of cargo\n";
        }

    }
    else if (tanker_state_ == state_unloading)
    {
        if (cargo_ == 0)
        {
            Ship::set_destination_island_and_speed(load_destination_, get_maximum_speed());
            tanker_state_ = state_moving_loading;
        }
        else
        {
            get_destination_island()->accept_fuel(cargo_);
            cargo_ = 0;
        }
    }


}

void Tanker::describe() const
{
    cout << "\nTanker ";
    Ship::describe();
    cout << "Cargo: " << cargo_ << " tons, ";

    switch (tanker_state_)
    {
    case state_no_cargo:
        cout << "no cargo destinations"; break;
    case state_moving_loading:
        cout << "moving to loading destination"; break;
    case state_moving_unloading:
        cout << "moving to unloading destination"; break;
    case state_loading:
        cout << "loading"; break;
    case state_unloading:
        cout << "unloading"; break;
    default:
        cout << "Error unhandled state"; break;
    }
    cout << endl;
}
