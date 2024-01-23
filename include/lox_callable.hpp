#ifndef LOX_CALLABLE_HPP
#define LOX_CALLABLE_HPP

#include "interpreter.hpp"


namespace lox {

class LoxCallable {

public:
    virtual size_t arity() = 0;
    virtual std::any call(Interpreter&, const std::shared_ptr<std::vector<std::any>>) = 0;
    virtual std::string to_string() = 0;

};


};

#endif
