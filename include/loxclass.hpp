#ifndef LOXCLASS_HPP
#define LOXCLASS_HPP

#include <string>
#include "lox_callable.hpp"


namespace lox {

class LoxClass : public LoxCallable, public std::enable_shared_from_this<LoxClass> {

public:

    const std::string name;

    LoxClass(const std::string name) : name(name) {}

    size_t arity() override;
    std::any call(Interpreter&, const std::shared_ptr<std::vector<std::any>>) override;
    std::string to_string() override;

};

};

#endif
