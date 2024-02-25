#ifndef LOX_INSTANCE_HPP
#define LOX_INSTANCE_HPP

#include "loxclass.hpp"


namespace lox {

class LoxInstance {
    
    std::shared_ptr<LoxClass> klass;

public:

    LoxInstance(std::shared_ptr<LoxClass> klass) : klass(klass) {}

    std::string to_string();

};

};

#endif
