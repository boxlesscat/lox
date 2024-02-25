#ifndef LOX_INSTANCE_HPP
#define LOX_INSTANCE_HPP

#include <string>
#include <unordered_map>

#include "lox_class.hpp"


namespace lox {

class LoxInstance : public std::enable_shared_from_this<LoxInstance> {
    
    std::shared_ptr<LoxClass> klass;
    std::unordered_map<std::string, std::any> fields;

public:

    LoxInstance(std::shared_ptr<LoxClass> klass) : klass(klass) {}

    std::any get(const Token name);
    void set(const Token name, std::any value);
    std::string to_string();

};

};

#endif
