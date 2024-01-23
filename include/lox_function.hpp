#ifndef LOX_FUNCTION_HPP
#define LOX_FUNCTION_HPP

#include "lox_callable.hpp"


namespace lox {

class LoxFunction : public LoxCallable {

    const std::shared_ptr<FnStmt> declaration;
    const std::shared_ptr<Environment> closure;

public:
    LoxFunction(const std::shared_ptr<FnStmt> declaration, const std::shared_ptr<Environment> closure) :
        declaration(declaration), closure(closure) {}

    size_t arity() override;
    std::any call(Interpreter&, std::shared_ptr<std::vector<std::any>>) override;
    std::string to_string() override;

};

};

#endif
