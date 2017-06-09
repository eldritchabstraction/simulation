#include <iostream>

#include "model.h"
#include "ship_factory.h"

using namespace std;

model::model() : time(0)
{
	new island("Exxon", point(10, 10), 1000, 200);
	new island("Shell", point(0, 30), 1000, 200);
	new island("Bermuda", point(20, 20));
	
	create_ship("Ajax", "Cruiser", point (15, 15));
	create_ship("Xerxes", "Cruiser", point (25, 25));
	create_ship("Valdez", "Tanker", point (30, 30));

	cout << "Model constructed" << endl;
}

island* model::get_island_ptr(const std::string& name) const {
}

ship* model::get_ship_ptr(const std::string& name) const {
}

void model::describe() const {
}

void model::update() {
}

void model::add_ship(ship*) {
}

void model::attach(view*) {
}

void model::detach(view*) {
}

void model::notify_location(const std::string& name, point location) {
}

void model::notify_gone(const std::string& name) {
}
