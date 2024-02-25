#include "lox_instance.hpp"
#include "error.hpp"


std::any lox::LoxInstance::get(const Token name) {
    if (fields.contains(name.lexeme))
        return fields[name.lexeme];
    throw RuntimeError(name, "Undefined Property");
}

void lox::LoxInstance::set(const Token name, std::any value) {
    fields[name.lexeme] = value;
}

std::string lox::LoxInstance::to_string() {
    return "<" + klass -> name + " instance>";
}
