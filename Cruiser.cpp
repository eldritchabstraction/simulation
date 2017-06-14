#include "Cruiser.h"
#include "Ship.h"
#include "Sim_object.h"
#include "Model.h"

using namespace std;

const double cruiser_range_max = 15;
const double cruiser_fire_power = 3;

Cruiser::Cruiser(const std::string& name, Point position): Ship(name, position, 1000, 20, 10, 6)
{
    fire_power_ = cruiser_fire_power;
    range_max_ = cruiser_range_max;

    cruiser_state_ = state_not_attacking;

    target_ = nullptr;

    cout << "\nCruiser " << name << " constructed\n";
}

Cruiser::~Cruiser()
{
    cout << "Cruiser " << name() << " destructed\n";
}

void Cruiser::update()
{
    Ship::update();

    switch(cruiser_state_)
    {
    case state_not_attacking:
        break;
    case state_attacking:
        if ((state_ & state_submerged) || !(target_->is_afloat()))
        {
            stop_attack();
            break;
        }

        cout << name() << " is attacking" << endl;

        if ((cartesian_distance(location(), target_->location())) < range_max_)
        {
            cout << name() << " fires\n";
            target_->receive_hit(fire_power_, this);
        }
        else
        {
            cout << name() << " target is out of range\n";
            stop_attack();
        }
        break;
    }
}

void Cruiser::describe() const
{
    cout << "\nCruiser ";
    Ship::describe();
}

void Cruiser::receive_hit(int hit, Ship* attacker)
{
    Ship::receive_hit(hit, attacker);

    if (cruiser_state_ != state_attacking)
        attack(attacker);
}

void Cruiser::attack(Ship* target)
{
    if (state_ & state_submerged)
        throw(runtime_error("Cannot attack!"));

    if (target == this)
        throw(runtime_error("can not attack itself"));

    if (target == target_)
        throw(runtime_error("already attacking this target"));

    target_ = target;
    cruiser_state_ = state_attacking;

    cout << name() << " will attack " << target->name() << endl;
}

void Cruiser::stop_attack()
{
    if (cruiser_state_ != state_attacking)
        throw(runtime_error("Was not attacking"));

    cruiser_state_ = state_not_attacking;
    target_ = nullptr;
    cout << name() << " stopping attack\n";


}
