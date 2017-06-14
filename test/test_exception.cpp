
/*
 * eldr: I wasn't sure if we could throw errors from within functions, so I just
 * wanted to test it out
 *
 *
 */
#include "../utility.h"
#include <stdexcept>

#include <iostream>
#include <string>

void erroneous (void)
{
    try
    {
        throw(std::runtime_error("oh nuts!"));
    }
    catch (std::runtime_error &e)
    {
        throw(std::runtime_error("oh noes!"));
    }
}

double validate_double(std::string input)
{
    try
    {
        try
        {
            double number = std::stol(input, nullptr, 10);
        }
        catch (std::invalid_argument &e)
        {
            throw(std::runtime_error("invalid argument caught"));
        }
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int validate_int(std::string input)
{
    try
    {
        try
        {
            int number = std::stoi(input, nullptr, 10);
        }
        catch (std::invalid_argument &e)
        {
            throw(std::runtime_error("invalid argument caught"));
        }
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int main (void)
{
    if (validate_int(std::string("10")))
    {
        std::cout << "10 invalid\n";
        return 1;
    }
    else if (validate_int(std::string("p")))
    {
        std::cout << "p invalid\n";
        return 1;
    }

}
