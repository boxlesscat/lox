#include "environment.hpp"
#include "error.hpp"


void lox::Environment::define(const lox::Token name, const std::any value) {
    if (enclosing == nullptr or values.find(name.lexeme) == values.end())
        values[name.lexeme] = value;
}

void lox::Environment::assign(const lox::Token name, const std::any value) {
    if (values.find(name.lexeme) != values.end())
        values[name.lexeme] = value;
    else if (enclosing != nullptr)
        enclosing -> assign(name, value);
    else
        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'");
}

std::any lox::Environment::get(const lox::Token name) {
    if (values.find(name.lexeme) != values.end())
        return values[name.lexeme];
    if (enclosing != nullptr)
        return enclosing -> get(name);
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'");
}
