
/* *** Use this function to calculate the subscripts for the cell. */

/* *** This code assumes the specified private member variables. */

#include <cmath>

#include "view.h"

// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type point,
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool view::get_subscripts(int &ix, int &iy, point location) const
{
	// adjust with origin and scale
	cartesian_vector subscripts = (location - origin) / scale;
	// Truncate coordinates to appropriate integer after taking the floor.
	// The floor function will produce the greatest integer that is 
	// smaller than the argument, even for negative values. 
	// So - 0.05 is floored to -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
		}
	return true;
}

