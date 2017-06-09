
/* create some islands and ships using the following code in the Model constructor.
Do not change the execution order of these code fragments. You should delete this comment. */

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

