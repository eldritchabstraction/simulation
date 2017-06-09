#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H
/* The Sim_object class provides the interface for all of simulation objects. 
It also stores the object's name, and has pure virtual accessor functions for 
the object's position and other information. */

/* *** You may not add any additional classes, structs, functions etc to this file. */
#include <string>
struct point;

class sim_object {
public:
    // *** define the constructor in Sim_object.cpp to output the supplied message
    // TODO: implement
	sim_object(const std::string& name) {}

    // *** define the destructor in Sim_object.cpp to output the supplied message
    // TODO: implement
    virtual ~sim_object() {}

	const std::string& get_name() const { return name_; }
    
	/* Interface for derived classes */
	// Ask model to notify views of current state
    virtual void broadcast_current_state() const = 0;

	virtual point get_location() const = 0;
	virtual void describe() const = 0;
	virtual void update() = 0;
	
	// Sim_objects must be unique, so disable copy/move construction, assignment
    // of base class; this will disable these operations for derived classes also.

private:
	std::string name_;
};


#endif
