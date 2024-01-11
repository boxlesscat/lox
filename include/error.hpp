#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include "token.hpp"


namespace lox {

static bool hadError = false;
static bool hadRuntimeError = false;

void error(const std::string, const int);

void error(const Token, std::string&&);

struct RuntimeError : public std::runtime_error {
    const Token token;
    RuntimeError(const Token token, const std::string&& message) : token(token), std::runtime_error(message) {}
};

void runtime_error(const RuntimeError);

};

#endif
