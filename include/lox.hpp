#ifndef LOX_HPP
#define LOX_HPP

#include <string>
#include "token.hpp"


namespace lox {

static bool hadError = false;

void run_file(const std::string&);

void run_prompt();

void run(const std::string&);

void error(const std::string, const int);

void report(const int, const std::string, const std::string);

};

#endif
