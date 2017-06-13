#include "Model.h"

#include <iostream>

#include "Ship_factory.h"
#include "utility.h"

using namespace std;

Model::Model() : time_(0)
{
	new Island("Exxon", Point(10, 10), 1000, 200);
	new Island("Shell", Point(0, 30), 1000, 200);
	new Island("Bermuda", Point(20, 20));
	
	create_ship("Ajax", "Cruiser", Point (15, 15));
	create_ship("Xerxes", "Cruiser", Point (25, 25));
	create_ship("Valdez", "Tanker", Point (30, 30));

	cout << "Model constructed" << endl;
}

Island* Model::get_island_ptr(const std::string& name) const
{
    auto it = islands_.find(name);

    try
    {
    if (it == islands_.end())
        throw(std::runtime_error("Island not found"));
    else
        return it->second;
    }
    catch(std::runtime_error &e)
    {
        cout << e.what() << endl;
        return nullptr;
    }

}

Ship* Model::get_ship_ptr(const std::string& name) const
{
    auto it = ships_.find(name);

    if (it == ships_.end())
        return nullptr;
    else
        return it->second;
}

void Model::describe() const
{
    for (auto p : sim_objects_)
        p->describe();
}

void Model::update() {
    time_++;

    for (auto p : sim_objects_)
        p->update();
}

void Model::add_ship(Ship* ship)
{
    auto it = ships_.find(ship->name());

    try
    {
    if (it == ships_.end())
        // add the ship to the maps
        ships_.insert(pair<string, Ship*>(ship->name(), ship));
    else
        throw(runtime_error(err_object_dupe));
    }
    catch (runtime_error &e)
    {
        cout << e.what() << endl;
        delete ship;
        return;
    }
}

void Model::attach(View *view)
{
    views_.push_front(view);
}

void Model::detach(View *view)
{
    views_.remove(view);
}

void Model::notify_location(const std::string& name, Point location)
{
    for (auto p : views_)
        p->update_location(name, location);
}

void Model::notify_gone(const std::string& name)
{
    for (auto p : views_)
        p->update_remove(name);
}
