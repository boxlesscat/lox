#ifndef RETURN_HPP
#define RETURN_HPP

#include <exception>
#include <any>

namespace lox {

struct Return : std::exception {

    std::any value;

    Return(std::any value) : value(value) {}

};

};

#endif
