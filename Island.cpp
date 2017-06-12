#include <iostream>

#include "Island.h"
#include "Model.h"

extern Model *global_model;

using namespace std;

Island::~Island()
{
    cout << "Island " << this->get_name() << " destructed/n";
}

void Island::update()
{
    if (production_rate_ > 0)
        fuel_ += production_rate_;
    cout << "Island " << this->get_name() << " now has " << fuel_ << " tons/n";
}

void Island::describe() const
{
    cout << "Island " << this->get_name() << " at position " << position_.x << "," << position_.y <<
        " fuel available: " << fuel_  << " tons/n";
}

void Island::broadcast_current_state() const
{
    // eldr: islands can't go missing... right?
    global_model->notify_location(this->get_name(), position_);
}

double Island::provide_fuel(double request)
{
    double return_fuel;
    if (fuel_ < request)
    {
        return_fuel = fuel_;
        fuel_ = 0;
    }
    else
    {
        return_fuel = request;
        fuel_ -= request;
    }

    return return_fuel;
}

void Island::accept_fuel(double amount)
{
    fuel_ += amount;
}
