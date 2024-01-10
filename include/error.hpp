#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include "token.hpp"


namespace lox {

static bool hadError = false;

void error(const std::string, const int);

void error(const Token, std::string&&);

};

#endif
