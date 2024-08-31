#ifndef LOX_FUNCTION_HPP
#define LOX_FUNCTION_HPP

#include "lox_callable.hpp"

namespace lox {

class LoxInstance;

class LoxFunction : public LoxCallable {

    FnStmt&                      declaration;
    std::shared_ptr<Environment> closure;

    bool is_init;

public:
    LoxFunction(FnStmt& declaration, std::shared_ptr<Environment> closure, bool is_init)
        : declaration(declaration), closure(std::move(closure)), is_init(is_init) {}

    size_t                       arity() override;
    std::shared_ptr<LoxFunction> bind(std::shared_ptr<LoxInstance>);
    Value                        call(Interpreter&, std::vector<Value>&) override;
    std::string                  to_string() const override;
};

};

#endif
