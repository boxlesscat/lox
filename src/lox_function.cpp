#include "environment.hpp"
#include "lox_function.hpp"
#include "lox_instance.hpp"
#include "return.hpp"


std::any lox::LoxFunction::call(Interpreter& interpreter, const std::shared_ptr<std::vector<std::any>> arguments) {
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    for (int i = 0; i < arguments -> size(); i++)
        environment -> define((*declaration -> params)[i].lexeme, (*arguments)[i]);
    try {
        interpreter.execute_block(declaration -> body, environment);
    } catch (Return value) {
        return value.value;
    }
    return nullptr;
}

size_t lox::LoxFunction::arity() {
    return declaration -> params -> size();
}

std::shared_ptr<lox::LoxFunction> lox::LoxFunction::bind(const std::shared_ptr<LoxInstance> instance) {
    std::shared_ptr<Environment> enivornment = std::make_shared<Environment>(closure);
    enivornment -> define("this", instance);
    return std::make_shared<LoxFunction>(declaration, enivornment);
}

std::string lox::LoxFunction::to_string() {
    return "<fn " + declaration -> name.lexeme + ">";
}
