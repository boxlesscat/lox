#ifndef LOX_INSTANCE_HPP
#define LOX_INSTANCE_HPP

#include "lox_class.hpp"

#include <string>
#include <unordered_map>

namespace lox {

class LoxInstance : public std::enable_shared_from_this<LoxInstance> {

    std::shared_ptr<LoxClass>              klass;
    std::unordered_map<std::string, Value> fields;

public:
    LoxInstance(std::shared_ptr<LoxClass> klass) : klass(std::move(klass)) {}

    Value       get(const Token& name);
    void        set(const Token& name, Value value);
    std::string to_string() const;
};

};

#endif
