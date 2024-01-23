#include "lox_function.hpp"
#include "return.hpp"


std::any lox::LoxFunction::call(Interpreter& interpreter, const std::shared_ptr<std::vector<std::any>> arguments) {
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(interpreter.globals);
    for (int i = 0; i < arguments -> size(); i++)
        environment -> define((*declaration -> params)[i], (*arguments)[i]);
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

std::string lox::LoxFunction::to_string() {
    return "<fn " + declaration -> name.lexeme + ">";
}
