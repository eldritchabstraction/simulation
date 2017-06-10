#include <algorithm>
#include <cctype>
#include <vector>

#include "utility.h"


int validate_ship_name(std::string name) {
    std::vector<std::string> command_list = {"open", "close", "default",
        "size", "zoom", "pan", "show", "quit", "status", "go", "create"};
    if (name.length() < 2)
        return 1;
    else if (std::find_if(name.begin(), name.end(),
                          [](const char &c) { return !std::isalnum(c); }) != name.end())
        return 1;
    else if (std::find_if(command_list.begin(), command_list.end(),
                          compare_ftor(name)) != command_list.end())
        return 1;

    return 0;
}
