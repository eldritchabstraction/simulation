#ifndef UTILITIES_H
#define UTILITIES_H
#include <exception>
#include <string>

class error : public std::exception {
public:
	error(std::string msg_) : msg(msg_.c_str()) {}
    const char* what() const noexcept override
        {return msg;}
private:
	const char* msg;
};

/* add any of your own declarations here */

#define const_string(x,y) const std::string x = y

const_string(err_expect_int         , "Expected an integer!");
const_string(err_expect_double      , "Expected a double!");


#endif
