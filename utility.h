#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>
#include <type_traits>

#define const_string(x,y) const std::string x = y
const_string(err_expect_int         , "Expected an integer!");
const_string(err_expect_double      , "Expected a double!");

/*
class error : public std::exception {
public:
	error(std::string msg_) : msg(msg_.c_str()) {}
    const char* what() const noexcept override
        {return msg;}
private:
	const char* msg;
};
*/

/* add any of your own declarations here */
class compare_ftor
{
public:
    compare_ftor(std::string string) : string_(string) {}
    bool operator()(const std::string &input) { return input == string_; }

protected:
    std::string string_;
};

template <class T>
int string_to_T(const std::string &input, T &output)
{
    T number;
    try
    {
        try
        {
            if (std::is_same<T, int>::value)
                number = std::stoi(input, nullptr, 10);
            else if (std::is_same<T, double>::value)
                number = std::stol(input, nullptr, 10);
            else
                throw(std::runtime_error("Error: unhandled type"));
        }
        catch (std::invalid_argument &e)
        {
            throw(std::runtime_error(err_expect_int));
        }
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    output = number;
    return 0;
}

/*
 * - cannot be shorter than 2 chars
 * - cannot contain characters that are not letters or numbers
 * - cannot be the same as command name for model or views
 */
int validate_ship_name(std::string name);



#endif
