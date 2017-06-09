/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Islands and Ships there are, but it does not
know about any of their derived classes, nor which Ships are of what kind of Ship. 
It has facilities for looking up objects by name, and removing Ships.  When
created, it creates an initial group of Islands and Ships using the Ship_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.
*/

/* 
This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/

#ifndef MODEL_H
#define MODEL_H

#include <string>

#include "island.h"
#include "geometry.h"
#include "ship.h"
#include "view.h"


class model {
public:
	// create the initial objects, output constructor message
	model();
	// destroy all objects, output destructor message
    // TODO: implement
	~model() {}

	// return the current time
	int get_time() {return time;}

	// Will throw Error("Island not found!") if no island of that name
	island* get_island_ptr(const std::string& name) const;

	// Returns nullptr if there is no ship of that name
	ship* get_ship_ptr(const std::string& name) const;
	
	// tell all objects to describe themselves
	void describe() const;
	// increment the time, and tell all objects to update themselves
	void update();	
	
 	// Add a new ship to the containers, and update the view
	// Throws Error if there is already a Ship or Island with that name.
	// If insertion fails for any exception, the pointed-to Ship is deleted
	// and the exception rethrown.
	void add_ship(ship*);

    /* Note: In Project 4 there is only one View. There will be multiple View objects
    later. So implement the View services so that multiple Views are possible by
    using a container of View pointers.  You should delete this comment. */
    
	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information.
	void attach(view*);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(view*);
	
    // notify the views about an object's location
	void notify_location(const std::string& name, point location);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);

	// disallow copy/move construction or assignment

private:
	int time;		// the simulated time


};
#endif
