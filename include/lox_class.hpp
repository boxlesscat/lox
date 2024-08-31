#ifndef LOXCLASS_HPP
#define LOXCLASS_HPP

#include "lox_callable.hpp"
#include "lox_function.hpp"

#include <string>
#include <unordered_map>

namespace lox {

class LoxClass : public LoxCallable, public std::enable_shared_from_this<LoxClass> {

    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
    std::shared_ptr<LoxClass>                                     superclass = nullptr;

public:
    const std::string name;

    LoxClass(std::string name, std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods, std::shared_ptr<LoxClass> superclass = nullptr)
        : name(std::move(name)), methods(std::move(methods)), superclass(std::move(superclass)) {}

    size_t                       arity() override;
    Value                        call(Interpreter&, std::vector<Value>&) override;
    std::shared_ptr<LoxFunction> find_method(const std::string&);
    std::string                  to_string() const override;
};

};

#endif
