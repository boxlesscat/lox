#include "loxclass.hpp"
#include "lox_instance.hpp"


size_t lox::LoxClass::arity() {
    return 0;
}

std::any lox::LoxClass::call(lox::Interpreter& interpreter, const std::shared_ptr<std::vector<std::any>> arguments) {
    return std::make_shared<LoxInstance>(shared_from_this());
}

std::string lox::LoxClass::to_string() {
    return "<class " + name + ">";
}
