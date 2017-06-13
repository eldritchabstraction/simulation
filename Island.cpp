#include <iostream>

#include "Island.h"
#include "Model.h"

extern Model *global_model;

using namespace std;

Island::~Island()
{
    cout << "Island " << name() << " destructed/n";
}

void Island::update()
{
    if (production_rate_ > 0)
        fuel_ += production_rate_;
    cout << "Island " << name() << " now has " << fuel_ << " tons/n";
}

void Island::describe() const
{
    cout << "\nIsland " << name() << " at position " << location() << endl;
    cout << "Fuel available: " << fuel_  << " tons\n";
}

void Island::broadcast_current_state() const
{
    // eldr: islands can't go missing... right?
    global_model->notify_location(this->name(), position_);
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
