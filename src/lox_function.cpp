#include "lox_function.hpp"

#include "environment.hpp"
#include "lox_instance.hpp"
#include "return.hpp"

lox::Value lox::LoxFunction::call(Interpreter& interpreter, std::vector<Value>& arguments) {
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    for (int i = 0; i < arguments.size(); i++)
        environment->define((declaration.params)[i].lexeme, arguments[i]);
    try {
        interpreter.execute_block(declaration.body, std::move(environment));
    } catch (Return& value) {
        if (is_init)
            return closure->get_at(0, "this");
        return value.value;
    }
    if (is_init)
        return closure->get_at(0, "this");
    return {};
}

size_t lox::LoxFunction::arity() {
    return declaration.params.size();
}

std::shared_ptr<lox::LoxFunction> lox::LoxFunction::bind(std::shared_ptr<LoxInstance> instance) {
    std::shared_ptr<Environment> enivornment = std::make_shared<Environment>(closure);
    enivornment->define("this", std::move(instance));
    return std::make_shared<LoxFunction>(declaration, std::move(enivornment), is_init);
}

std::string lox::LoxFunction::to_string() const {
    return "<fn " + declaration.name.lexeme + ">";
}
