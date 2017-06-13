/*
Main module. Your project must use this file as-is
*/


#include <iostream>

#include "Controller.h"
#include "Model.h"

using namespace std;

Model *global_model;

// The main function creates the Model and Controller objects, 
// then tells the Controller to run. 

int main ()
{	
	// Set output to show two decimal places for floating point numbers
//	cout << fixed << setprecision(2) << endl;
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2);

	// create a Model object;
    global_model = new Model;
	// create the Controller object and go
	Controller c;

	c.run();
    
    delete global_model;
}

