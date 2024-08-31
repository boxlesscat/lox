#include "interpreter.hpp"

#include "error.hpp"
#include "lox_class.hpp"
#include "lox_function.hpp"
#include "lox_instance.hpp"
#include "return.hpp"

#include <chrono>

struct Clock : public lox::LoxCallable {

    size_t arity() override {
        return 0;
    }

    lox::Value call(lox::Interpreter& interpreter, std::vector<lox::Value>& args) override {
        auto ticks = std::chrono::system_clock::now().time_since_epoch();
        return std::chrono::duration<double>(ticks).count();
    }

    std::string to_string() const {
        return "<fn clock>";
    }
};

lox::Interpreter::Interpreter() {
    std::shared_ptr<LoxCallable> clk = std::make_shared<Clock>();
    globals->define("clock", std::move(clk));
}

bool lox::Interpreter::is_truthy(const lox::Value& value) const {
    if (std::holds_alternative<std::monostate>(value))
        return false;
    if (std::holds_alternative<bool>(value))
        return get<bool>(value);
    if (std::holds_alternative<double>(value))
        return get<double>(value);
    if (std::holds_alternative<std::string>(value))
        return !get<std::string>(value).empty();
    return false;
}

bool lox::Interpreter::is_equal(const Value& l, const Value& r) const {
    return std::visit(
        [&](const auto& l) -> bool {
            return std::visit(
                [&](const auto& r) -> bool {
                    if constexpr (std::is_same_v<decltype(l), decltype(r)>)
                        return l == r;
                    return false;
                },
                r
            );
        },
        l
    );
}

void lox::Interpreter::check_number_operand(const lox::Token& op, const lox::Value& operand) const {
    if (std::holds_alternative<double>(operand))
        return;
    throw RuntimeError(op, "Operand must be a number");
}

void lox::Interpreter::check_number_operands(const lox::Token& op, const lox::Value& left, const lox::Value& right) const {
    if (std::holds_alternative<double>(left) and std::holds_alternative<double>(right))
        return;
    throw RuntimeError(op, "Operands must be numbers");
}

void lox::Interpreter::execute(std::unique_ptr<lox::Stmt>& statement) {
    statement->accept(*this);
}

void lox::Interpreter::execute_block(std::vector<std::unique_ptr<lox::Stmt>>& statements, std::shared_ptr<Environment> environment) {
    std::shared_ptr<Environment> previous = std::move(this->environment);
    try {
        this->environment = std::move(environment);
        for (auto& statement : statements)
            execute(statement);
        this->environment = std::move(previous);
    } catch (...) {
        this->environment = std::move(previous);
        throw;
    }
}

lox::Value lox::Interpreter::evaluate(lox::Expr& expr) {
    return expr.accept(*this);
}

lox::Value lox::Interpreter::evaluate(std::unique_ptr<lox::Expr>& expr) {
    return expr->accept(*this);
}

lox::Value lox::Interpreter::visit(lox::AssignExpr& expr) {
    Value value = evaluate(expr.value);
    environment->assign(expr.name, std::move(value));
    return value;
}

lox::Value lox::Interpreter::visit(lox::BinaryExpr& expr) {
    Value left  = evaluate(expr.left);
    Value right = evaluate(expr.right);
    switch (expr.op.type) {
    case BANG_EQUAL:
        return !is_equal(left, right);
    case EQUAL_EQUAL:
        return is_equal(left, right);
    case MINUS:
    case STAR:
    case SLASH:
    case GREATER:
    case GREATER_EQUAL:
    case LESSER:
    case LESSER_EQUAL: {
        check_number_operands(expr.op, left, right);
        const double l = std::get<double>(left);
        const double r = std::get<double>(right);
        switch (expr.op.type) {
        case MINUS:
            return l - r;
        case STAR:
            return l * r;
        case SLASH:
            return l / r;
        case GREATER:
            return l > r;
        case GREATER_EQUAL:
            return l >= r;
        case LESSER:
            return l < r;
        case LESSER_EQUAL:
            return l <= r;
        }
    }
    case PLUS:
        if (std::holds_alternative<double>(left) and std::holds_alternative<double>(right))
            return std::get<double>(left) + std::get<double>(right);
        if (std::holds_alternative<std::string>(left) or std::holds_alternative<std::string>(right))
            return stringfy(left) + stringfy(right);
        throw RuntimeError(expr.op, "cannot add " + stringfy(left) + " and " + stringfy(right));
    }
    return "\n(binary expr) something's wrong if you can see this\n";
}

lox::Value lox::Interpreter::visit(lox::CallExpr& expr) {
    Value        callee = evaluate(expr.callee);
    LoxCallable* function;
    try {
        function = std::get<std::shared_ptr<LoxCallable>>(callee).get();
    } catch (...) {
        throw RuntimeError(expr.paren, "Can only call functions and methods");
    }
    if (expr.arguments.size() != function->arity())
        throw RuntimeError(
            expr.paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(expr.arguments.size())
        );
    std::vector<Value> arguments;
    for (auto& args : expr.arguments)
        arguments.emplace_back(evaluate(args));
    return function->call(*this, arguments);
}

lox::Value lox::Interpreter::visit(lox::GetExpr& expr) {
    Value object = evaluate(expr.object);
    if (!std::holds_alternative<std::shared_ptr<LoxInstance>>(object))
        throw RuntimeError(expr.name, "Only instances have properties.");
    return std::get<std::shared_ptr<LoxInstance>>(object)->get(expr.name);
}

lox::Value lox::Interpreter::visit(lox::GroupingExpr& expr) {
    return evaluate(expr.expr);
}

lox::Value lox::Interpreter::visit(lox::LiteralExpr& expr) {
    return expr.value; // do not move this since we might want to refer to the value again in a loop
}

lox::Value lox::Interpreter::visit(lox::LogicalExpr& expr) {
    Value left = evaluate(expr.left);
    if (expr.op.type == OR) {
        if (is_truthy(left))
            return std::move(left);
    } else { // and branch
        if (!is_truthy(left))
            return std::move(left);
    }
    return evaluate(expr.right);
}

lox::Value lox::Interpreter::visit(lox::SetExpr& expr) {
    Value object = evaluate(expr.object);
    if (!std::holds_alternative<std::shared_ptr<LoxInstance>>(object))
        throw RuntimeError(expr.name, "Only instances have fields");
    Value value = evaluate(expr.value);
    std::get<std::shared_ptr<LoxInstance>>(object).get()->set(expr.name, value);
    return value;
}

lox::Value lox::Interpreter::visit(lox::SuperExpr& expr) {
    const int                 distance = locals[&expr];
    std::shared_ptr<LoxClass> superclass =
        std::dynamic_pointer_cast<LoxClass>(std::get<std::shared_ptr<LoxCallable>>(environment->get_at(distance, "super")));
    std::shared_ptr<LoxInstance> object = std::get<std::shared_ptr<LoxInstance>>(environment->get_at(distance - 1, "this"));
    std::shared_ptr<LoxFunction> method = superclass->find_method(expr.method.lexeme);
    if (method == nullptr)
        throw RuntimeError(expr.method, "Undefined property '" + expr.method.lexeme + "'");
    return method->bind(std::move(object));
}

lox::Value lox::Interpreter::visit(lox::ThisExpr& expr) {
    return lookup_variable(expr.keyword, &expr);
}

lox::Value lox::Interpreter::visit(UnaryExpr& expr) {
    Value right = evaluate(expr.right);
    switch (expr.op.type) {
    case BANG:
        return std::move(!is_truthy(right));
    case MINUS:
        check_number_operand(expr.op, right);
        return -get<double>(right);
    }
    return "\n(unary expr) something's wrong if you can see this\n";
}

lox::Value lox::Interpreter::visit(lox::VariableExpr& expr) {
    return lookup_variable(expr.name, &expr);
}

void lox::Interpreter::visit(lox::BlockStmt& statement) {
    execute_block(statement.statements, std::make_shared<Environment>(environment));
}

void lox::Interpreter::visit(lox::ClassStmt& statement) {
    Value                     superclass;
    std::shared_ptr<LoxClass> superclassptr;
    if (statement.superclass) {
        superclass = evaluate(*statement.superclass.get());
        if (!std::holds_alternative<std::shared_ptr<LoxCallable>>(superclass))
            throw RuntimeError(statement.superclass->name, "superclass must be a class");
        if (auto sc = std::dynamic_pointer_cast<LoxClass>(std::get<std::shared_ptr<LoxCallable>>(superclass)); !sc)
            throw RuntimeError(statement.superclass->name, "superclass must be a class");
        else
            superclassptr = sc;
    }
    if (statement.superclass) {
        environment = std::make_shared<Environment>(environment);
        environment->define("super", superclass);
    }
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
    for (auto& method : statement.methods)
        methods[method->name.lexeme] = std::make_shared<LoxFunction>(*method.get(), environment, method->name.lexeme == "init");
    std::shared_ptr<LoxClass> klass = std::make_shared<LoxClass>(statement.name.lexeme, methods, superclassptr);
    if (superclassptr)
        environment = environment->enclosing;
    environment->define(statement.name.lexeme, klass);
}

void lox::Interpreter::visit(lox::FnStmt& statement) {
    environment->define(statement.name.lexeme, std::make_shared<LoxFunction>(statement, environment, false));
}

void lox::Interpreter::visit(lox::IfStmt& statement) {
    if (is_truthy(evaluate(statement.condition)))
        execute(statement.then);
    else if (statement.otherwise)
        execute(statement.otherwise);
}

void lox::Interpreter::visit(lox::PrintStmt& statement) {
    Value value = evaluate(statement.expr);
    std::cout << stringfy(value) << "\n";
}

void lox::Interpreter::visit(lox::ExprStmt& statement) {
    evaluate(statement.expr);
}

void lox::Interpreter::visit(lox::VarStmt& statement) {
    Value value;
    if (statement.initializer != nullptr)
        value = evaluate(statement.initializer);
    environment->define(statement.name.lexeme, value);
}

void lox::Interpreter::visit(lox::ReturnStmt& statement) {
    throw Return(evaluate(statement.value));
}

void lox::Interpreter::visit(lox::WhileStmt& statement) {
    while (is_truthy(evaluate(statement.condition)))
        execute(statement.body);
}

lox::Value lox::Interpreter::lookup_variable(const lox::Token& name, lox::Expr* expr) {
    if (locals.contains(expr))
        return environment->get_at(locals[expr], name.lexeme);

    return globals->get(name);
}

void lox::Interpreter::interpret(std::vector<std::unique_ptr<lox::Stmt>>& statements) {
    try {
        for (auto& statement : statements)
            execute(statement);
    } catch (RuntimeError error) {
        runtime_error(error);
    }
}

void lox::Interpreter::resolve(lox::Expr* expr, const int depth) {
    locals[expr] = depth;
}

std::string lox::Interpreter::stringfy(const lox::Value& value) const {
    if (std::holds_alternative<std::monostate>(value))
        return "nil";
    if (std::holds_alternative<bool>(value))
        return is_truthy(value) ? "true" : "false";
    if (std::holds_alternative<double>(value))
        return std::to_string(std::get<double>(value));
    if (std::holds_alternative<std::string>(value))
        return std::get<std::string>(value);
    if (std::holds_alternative<std::shared_ptr<LoxCallable>>(value))
        return std::get<std::shared_ptr<LoxCallable>>(value)->to_string();
    if (std::holds_alternative<std::shared_ptr<LoxInstance>>(value))
        return std::get<std::shared_ptr<LoxInstance>>(value)->to_string();
    return "\n(stringify) something's wrong. this should not be reachable\n";
}
