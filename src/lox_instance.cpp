#include "lox_function.hpp"
#include "lox_instance.hpp"
#include "error.hpp"


std::any lox::LoxInstance::get(const Token name) {
    if (fields.contains(name.lexeme))
        return fields[name.lexeme];
    std::shared_ptr<LoxFunction> method = klass -> find_method(name.lexeme);
    if (method != nullptr)
        return method;
    throw RuntimeError(name, "Undefined Property");
}

void lox::LoxInstance::set(const Token name, std::any value) {
    fields[name.lexeme] = value;
}

std::string lox::LoxInstance::to_string() {
    return "<" + klass -> name + " instance>";
}
