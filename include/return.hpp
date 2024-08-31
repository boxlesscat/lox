#ifndef RETURN_HPP
#define RETURN_HPP

#include "token.hpp"

#include <exception>

namespace lox {

struct Return : std::exception {

    Value value;

    Return(Value value) : value(std::move(value)) {}
};

};

#endif
