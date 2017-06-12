
/* *** Use this function to calculate the subscripts for the cell. */

/* *** This code assumes the specified private member variables. */

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>

#include "view.h"
#include "utility.h"

using std::vector;
using std::string; using std::stringstream;
using std::cout;

// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type point,
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool view::get_subscripts(int &ix, int &iy, point location) const
{
	// adjust with origin and scale
	cartesian_vector subscripts = (location - origin_) / scale_;
	// Truncate coordinates to appropriate integer after taking the floor.
	// The floor function will produce the greatest integer that is 
	// smaller than the argument, even for negative values. 
	// So - 0.05 is floored to -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size_) || (iy < 0) || (iy >= size_)) {
		return false;
		}
	return true;
}

// map defaults to 25 x 25, scale 2.0, origin(-10, -10)
void view::set_defaults()
{
    size_ = 25; scale_ = 2.0; origin_ = point(-10, -10);
}

void view::update_location(const std::string& name, point location)
{
    auto it = sim_objects_.find(name);

    if (it == sim_objects_.end())
        sim_objects_.insert(std::pair<string, point>(name, location));
    else
        sim_objects_[name] = location;
}

void view::update_remove(const std::string& name) {

    auto it = sim_objects_.find(name);
    if (it == sim_objects_.end())
        return;
    else
        sim_objects_.erase(it);
}

void view::generate(char_map_t &char_map) const
{
    int x = origin_.x, y = origin_.y;

    // generate row labels
    stringstream row_labels_stream;
    for (int i = 0; i < (size_ * scale_); i += 3 * scale_)
        row_labels_stream << std::setw(6) << (std::to_string(x + i));
    vector<string> row_labels = {row_labels_stream.str()};

    // our stategy is to make a copy of the sim_objects map and delete elements as we place them on
    // the grid, any elements remaining go on a special list "outside the map"
    std::map<string, point> sim_objects;
    sim_objects.insert(sim_objects_.begin(), sim_objects_.end());

    char_map.push_back(row_labels);
    for (int i = 0; i < size_; ++i)
    {
        stringstream row_stream;
        // push back column axis labels
        if (i % 3 == 0)
        {
            double column_label = y + i * scale_;
            row_stream << std::setw(4) << std::setprecision(3) << column_label << " ";
        }
        else
            row_stream << "     ";
        for (int j = 0; j < size_; ++j)
        {
            // by default, we will be pushing back a blank
            string cursor_value = ". ";
            double cursor_x = j * scale_, cursor_y = i * scale_;
            // iterate through the list of objects and see if there's an object to place
            for (auto it = sim_objects.begin(); it != sim_objects.end(); ++it)
            {
                auto object = *it;
                string object_name = object.first;
                point object_point = object.second;
                double object_x = object_point.x, object_y = object_point.y;
                // the match won't normally be precise, we basically just need the object to be in
                // the "box" defined by the cursor
                if ((object_x >= (cursor_x + scale_)) && (object_x < (cursor_x + scale_)) &&
                    (object_y >= (cursor_y + scale_)) && (object_y < (cursor_y + scale_))
                   )
                {
                    if (cursor_value == ". ")
                        cursor_value = object_name.substr(0,1);
                    else
                        cursor_value = "* ";
                }
            }
            row_stream << cursor_value;
        }
        vector<string> row = { row_stream.str() };
        char_map.push_back(row);
    }
}

void view::draw() const
{
    char_map_t char_map;
    generate(char_map);

    for (auto it = char_map.rbegin(); it != char_map.rend(); ++it )
    {
        for (auto c : *it)
            std::cout << c;
        std::cout << std::endl;
    }
}

// map size must be between (6, 30]
void view::set_size(int size) {
    size_ = size;
}

// map scale must be greater than 0.0
void view::set_scale(double scale) {
    scale_ = scale;
}

void view::set_origin(point origin) {
    origin_ = origin;
}
