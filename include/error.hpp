#ifndef ERROR_HPP
#define ERROR_HPP

#include "token.hpp"

#include <string>

namespace lox {

extern bool had_error;
extern bool had_runtime_error;

void error(const std::string&, const int);

void error(const Token&, const std::string&);

struct RuntimeError : public std::runtime_error {
    const Token& token;
    RuntimeError(const Token& token, const std::string& message) : token(token), std::runtime_error(message) {}
};

void runtime_error(const RuntimeError&);

};

#endif
