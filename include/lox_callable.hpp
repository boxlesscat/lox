#ifndef LOX_CALLABLE_HPP
#define LOX_CALLABLE_HPP

#include "interpreter.hpp"

namespace lox {

class LoxCallable {

public:
    virtual size_t      arity()                                 = 0;
    virtual Value       call(Interpreter&, std::vector<Value>&) = 0;
    virtual std::string to_string() const                       = 0;
};

};

#endif
