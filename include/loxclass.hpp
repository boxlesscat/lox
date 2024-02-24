#ifndef LOXCLASS_HPP
#define LOXCLASS_HPP

#include <string>


namespace lox {

class LoxClass {

    const std::string name;

public:

    LoxClass(const std::string name) : name(name) {}

    std::string to_string();

};

};

#endif
