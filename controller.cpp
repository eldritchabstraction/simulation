#include <iostream>
#include <string>
#include <stdexcept>

#include <vector>

#include <cctype>
#include <algorithm>

#include "model.h"
#include "geometry.h"
#include "ship.h"
#include "ship_factory.h"
#include "utility.h"
#include "controller.h"

using std::cout; using std::endl;
using std::string;
using std::vector;

extern model *m;

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

controller::controller(): view_(NULL)
{
    // TODO: acquire a pointer to view
    cout << "controller constructing\n";
}

controller::~controller()
{
    cout << "controller destructing\n";
}

/*
 * run: a command loop where a user types in commands and they are carried out
 */
void controller::run(void)
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
                throw error("View is already open!");
            view_= new view();
        } else if (cmd == "close") {
            // detach the view from the model and destroy; not exist error
            if (!view_)
                throw error("View is not open!");
            delete view_;
        } else if (cmd == "default") {
            // restore settings of the map
            view_->set_defaults();
        } else if (cmd == "size") {
            // read a single integer for the size of the square map
            int size;
            try{
                size = std::stoi(split[1], nullptr, 10);
            } catch (std::invalid_argument &e) {
                throw error(err_expect_int);
            }
            view_->set_size(size);
        } else if (cmd == "zoom") {
            // read a double for the scale of the map (nm per cell)
            double scale;
            try{
                scale = std::stol(split[1], nullptr, 10);
            } catch (std::invalid_argument &e) {
                throw error(err_expect_double);
            }
            view_->set_scale(scale);
        } else if (cmd == "pan") {
            double x, y;
            try{
                x = std::stol(split[1], nullptr, 10);
                y = std::stol(split[1], nullptr, 10);
            } catch (std::invalid_argument &e) {
                throw error(err_expect_double);
            }
            view_->set_origin(point(x, y));
        } else if (cmd == "show") {
            if (!view_)
                throw error("View is not open!");
            view_->draw();
        //  *** model commands ***
        } else if (cmd == "quit") {
            break;
        } else if (cmd == "status") {
            m->describe();
        } else if (cmd ==  "go") {
            m->update();
        } else if (cmd == "create") {
            // TODO: fully implement
            // create_ship();
        }

        } catch (std::exception &e) {
            cout << e.what() << endl;

        }
    }

    cout << "Done\n";
}
