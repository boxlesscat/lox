#ifndef LOX_HPP
#define LOX_HPP

#include "token.hpp"

#include <string>

namespace lox {

void run_file(const std::string&);

void run_prompt();

void run(const std::string&);

void report(const int, const std::string&, const std::string&);

};

#endif
