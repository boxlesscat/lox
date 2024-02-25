#include "lox_instance.hpp"


std::string lox::LoxInstance::to_string() {
    return "<" + klass -> name + " instance>";
}
