#ifndef LOXCLASS_HPP
#define LOXCLASS_HPP

#include <string>
#include <unordered_map>

#include "lox_callable.hpp"
#include "lox_function.hpp"


namespace lox {

class LoxClass : public LoxCallable, public std::enable_shared_from_this<LoxClass> {

    const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<LoxFunction>>> methods;

public:

    const std::string name;

    LoxClass(const std::string name, const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<LoxFunction>>> methods) : name(name), methods(methods) {}

    size_t arity() override;
    std::any call(Interpreter&, const std::shared_ptr<std::vector<std::any>>) override;
    std::shared_ptr<LoxFunction> find_method(const std::string);
    std::string to_string() override;

};

};

#endif
