#include "lox_instance.hpp"

#include "error.hpp"
#include "lox_function.hpp"

lox::Value lox::LoxInstance::get(const Token& name) {
    if (fields.contains(name.lexeme))
        return fields[name.lexeme];
    std::shared_ptr<LoxFunction> method = klass->find_method(name.lexeme);
    if (method)
        return method->bind(shared_from_this());
    throw RuntimeError(name, "Undefined Property");
}

void lox::LoxInstance::set(const Token& name, lox::Value value) {
    fields[name.lexeme] = std::move(value);
}

std::string lox::LoxInstance::to_string() const {
    return "<" + klass->name + " instance>";
}
