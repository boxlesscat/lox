#include "lox_class.hpp"

#include "lox_function.hpp"
#include "lox_instance.hpp"

size_t lox::LoxClass::arity() {
    std::shared_ptr<LoxFunction> init = find_method("init");
    if (init == nullptr)
        return 0;
    return init->arity();
}

lox::Value lox::LoxClass::call(lox::Interpreter& interpreter, std::vector<lox::Value>& arguments) {
    std::shared_ptr<LoxInstance> instance = std::make_shared<LoxInstance>(shared_from_this());
    std::shared_ptr<LoxFunction> init     = find_method("init");
    if (init != nullptr)
        init->bind(instance)->call(interpreter, arguments);
    return instance;
}

std::shared_ptr<lox::LoxFunction> lox::LoxClass::find_method(const std::string& name) {
    if (methods.contains(name))
        return methods[name];
    if (superclass)
        return superclass->find_method(name);
    return {};
}

std::string lox::LoxClass::to_string() const {
    return "<class " + name + ">";
}
