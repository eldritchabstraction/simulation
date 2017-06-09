#ifndef SHIP_FACTORY_H
#define SHIP_FACTORY_H

#include <string>
struct point;

class ship;
/* This is a very simple form of factory, a function; you supply the information, it creates
the specified kind of object and returns a pointer to it. The Ship is allocated
with new, so some other component is resposible for deleting it.
*/

// may throw Error("Trying to create ship of unknown type!")
// TODO: implement
ship * create_ship(const std::string& name, const std::string& type, point initial_position) {};

#endif
