#include "Controller.h"

#include <iostream>
#include <string>
#include <stdexcept>

#include <vector>

#include <cctype>
#include <algorithm>

#include "geometry.h"
#include "utility.h"
#include "Model.h"
#include "Ship.h"
#include "Ship_factory.h"

using std::cout; using std::endl;
using std::string;
using std::vector;
using std::runtime_error;

extern Model *global_model;

vector<string> split_line(string input)
{
    vector<string> split;

    // two iterators it and jt both moving through the input and pointing at the
    // beginning and end of words

    auto it = input.begin(), jt = input.begin();;

    while (it != input.end())
    {
        it = find_if(it, input.end(), [](const char &c) { return !std::isspace(c); });
        jt = find_if(it, input.end(), [](const char &c) { return std::isspace(c); });

        split.push_back(string(it, jt));

        it = jt;
    }

    return split;
}

Controller::Controller(): view_(NULL)
{
    cout << "controller constructing\n";
}

Controller::~Controller()
{
    cout << "controller destructing\n";
}

/*
 * run: a command loop where a user types in commands and they are carried out
 */
void Controller::run(void)
{
    while (1)
    {
        string input_line;
        std::getline(cin, input_line);

        vector<string> split = split_line(input_line);

        /*
        if (is_ship(split[1]))
        {
            // do ship stuff
        }
        */

        try {

        std::string cmd = split[0];

        //  *** view commands ***
        if (cmd == "open")
        {
            // create the view; already exist error
            if (view_)
                throw runtime_error("View is already open!");
            view_= new View();
        } else if (cmd == "close") {
            // detach the view from the model and destroy; not exist error
            if (!view_)
                throw runtime_error("View is not open!");
            delete view_;
        } else if (cmd == "default") {
            // restore settings of the map
            view_->set_defaults();
        } else if (cmd == "size") {
            // read a single integer for the size of the square map
            int size;
            if (string_to_T<int>(split[1], size))
                continue;
            view_->set_size(size);
        } else if (cmd == "zoom") {
            // read a double for the scale of the map (nm per cell)
            double scale;
            if (string_to_T<double>(split[1], scale))
                continue;
            view_->set_scale(scale);
        } else if (cmd == "pan") {
            double x, y;
            if (string_to_T(split[1], x) || string_to_T(split[2], y))
                continue;
            view_->set_origin(Point(x, y));
        } else if (cmd == "show") {
            if (!view_)
                throw runtime_error("View is not open!");
            view_->draw();
        //  *** model commands ***
        } else if (cmd == "quit") {
            break;
        } else if (cmd == "status") {
            global_model->describe();
        } else if (cmd ==  "go") {
            global_model->update();
        } else if (cmd == "create") {
            double x, y;
            if (validate_ship_name(split[1]) || string_to_T(split[3], x) || string_to_T(split[4], y))
                continue;
            create_ship(split[1], split[2], Point(x,y));
        }

        } catch (std::exception &e) {
            cout << e.what() << endl;

        }
    }

    cout << "Done\n";
}
