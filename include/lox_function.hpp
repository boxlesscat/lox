#ifndef LOX_FUNCTION_HPP
#define LOX_FUNCTION_HPP

#include "lox_callable.hpp"


namespace lox {

class LoxInstance;

class LoxFunction : public LoxCallable {

    const std::shared_ptr<FnStmt> declaration;
    const std::shared_ptr<Environment> closure;

    bool is_init;

public:
    LoxFunction(const std::shared_ptr<FnStmt> declaration, const std::shared_ptr<Environment> closure, bool is_init) :
        declaration(declaration), closure(closure), is_init(is_init) {}

    size_t arity() override;
    std::shared_ptr<LoxFunction> bind(const std::shared_ptr<LoxInstance>);
    std::any call(Interpreter&, std::shared_ptr<std::vector<std::any>>) override;
    std::string to_string() override;

};

};

#endif
