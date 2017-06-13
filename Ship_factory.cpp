#include "geometry.h"
#include "Ship.h"
#include "Cruiser.h"
#include "Tanker.h"

Ship* create_ship(const std::string& name, const std::string& type, Point initial_position)
{
    if (type == "Cruiser")
        return new Cruiser(name, initial_position);
    else if (type == "Tanker")
        return new Tanker(name, initial_position);
}
