#include "environment.hpp"
#include "error.hpp"


void lox::Environment::define(const lox::Token name, const std::any value) {
    values[name.lexeme] = value;
}

void lox::Environment::assign(const lox::Token name, const std::any value) {
    if (values.contains(name.lexeme))
        values[name.lexeme] = value;
    else if (enclosing != nullptr)
        enclosing -> assign(name, value);
    else
        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'");
}

void lox::Environment::assign_at(const int distance, Token name, std::any value) {
    ancestor(distance) -> values[name.lexeme] = value;
}

std::shared_ptr<lox::Environment> lox::Environment::ancestor(const int distance) {
    std::shared_ptr<Environment> environment = shared_from_this();
    for (int i = 0; i < distance; i++)
        environment = environment -> enclosing;
    return environment;
}

std::any lox::Environment::get(const lox::Token name) {
    if (values.contains(name.lexeme))
        return values[name.lexeme];
    if (enclosing != nullptr)
        return enclosing -> get(name);
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'");
}

std::any lox::Environment::get_at(const int distance, const std::string name) {
    return ancestor(distance) -> values[name];
}
