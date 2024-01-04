#ifndef LOX_HPP
#define LOX_HPP

#include <vector>
#include <string>
#include "token.hpp"


namespace lox {

bool hadError = false;

std::vector<Token> scanTokens(const std::string&);

void run_file(const std::string&);

void run_prompt();

void run(const std::string&);

void error(const std::string&, const int);

void report(const std::string&, const int, const std::string);

};

#endif
