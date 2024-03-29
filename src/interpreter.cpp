#include "interpreter.hpp"
#include "error.hpp"
#include "lox_class.hpp"
#include "lox_instance.hpp"
#include "lox_function.hpp"
#include "return.hpp"
#include <chrono>


struct Clock :  public lox::LoxCallable {

    size_t arity() override {
        return 0;
    }

    std::any call(lox::Interpreter& interpreter, std::shared_ptr<std::vector<std::any>> args) override {
        auto ticks = std::chrono::system_clock::now().time_since_epoch();
        return std::chrono::duration<double>(ticks).count();
    }

    std::string to_string() {
        return "<fn clock>";
    }

};

lox::Interpreter::Interpreter() {
    std::shared_ptr<LoxCallable> clk = std::make_shared<Clock>();
    globals -> define("clock", clk);
}

bool lox::Interpreter::is_truthy(const std::any value) const {
    const auto& type = value.type();
    if (type == typeid(nullptr))
        return false;
    if (type == typeid(bool))
        return std::any_cast<bool>(value);
    if (type == typeid(double))
        return std::any_cast<double>(value);
    return true;
}

bool lox::Interpreter::is_equal(const std::any x, const std::any y) const {
    const auto& type = x.type();
    if (type != y.type())
        return false;
    if (type == typeid(nullptr))
        return true;
    if (type == typeid(double))
        return std::any_cast<double>(x) == std::any_cast<double>(y);
    if (type == typeid(bool))
        return std::any_cast<bool>(x) == std::any_cast<bool>(y);
    if (type == typeid(std::string))
        return std::any_cast<std::string>(x) == std::any_cast<std::string>(y);
    if (type == typeid(std::shared_ptr<LoxInstance>))
        return std::any_cast<std::shared_ptr<LoxInstance>>(x) == std::any_cast<std::shared_ptr<LoxInstance>>(y);
    if (type == typeid(std::shared_ptr<LoxClass>))
        return std::any_cast<std::shared_ptr<LoxClass>>(x) == std::any_cast<std::shared_ptr<LoxClass>>(y);
    if (type == typeid(std::shared_ptr<LoxCallable>))
        return std::any_cast<std::shared_ptr<LoxCallable>>(x) == std::any_cast<std::shared_ptr<LoxCallable>>(y);
    if (type == typeid(std::shared_ptr<LoxFunction>))
        return std::any_cast<std::shared_ptr<LoxFunction>>(x) == std::any_cast<std::shared_ptr<LoxFunction>>(y);
    return false;
}

void lox::Interpreter::check_number_operand(const lox::Token op, const std::any operand) const {
    if (operand.type() == typeid(double))
        return;
    throw RuntimeError(op, "Operand must be a number");
}

void lox::Interpreter::check_number_operands(const lox::Token op, const std::any left, const std::any right) const {
    if (left.type() == typeid(double) and right.type() == typeid(double))
        return;
    throw RuntimeError(op, "Operands must be numbers");
}

void lox::Interpreter::execute(const std::shared_ptr<lox::Stmt> statement) {
    statement -> accept(*this);
}

void lox::Interpreter::execute_block(const std::shared_ptr<std::vector<std::shared_ptr<lox::Stmt>>> statements, std::shared_ptr<lox::Environment> environment) {
    std::shared_ptr<Environment> previous = this -> environment;
    try {
        this -> environment = environment;
        for (auto statement : *statements)
            execute(statement);
        this -> environment = previous;
    } catch (...) {
        this -> environment = previous;
        throw;
    }
}

std::any lox::Interpreter::evaluate(const std::shared_ptr<lox::Expr> expr) {
    return expr -> accept(*this);
}

std::any lox::Interpreter::visit_assign_expr(const std::shared_ptr<lox::AssignExpr> expr) {
    std::any value = evaluate(expr -> value);
    environment -> assign(expr -> name, value);
    return value;
}

std::any lox::Interpreter::visit_binary_expr(const std::shared_ptr<lox::BinaryExpr> expr) {
    const std::any left = evaluate(expr -> left);
    const std::any right = evaluate(expr -> right);
    switch (expr -> op.type) {
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
            check_number_operands(expr -> op, left, right);
            const double l = std::any_cast<double>(left);
            const double r = std::any_cast<double>(right);
            switch (expr -> op.type) {
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
            if (left.type() == typeid(double) and right.type() == typeid(double))
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            if (left.type() == typeid(std::string) and right.type() == typeid(std::string))
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            throw RuntimeError(expr -> op, "Operands must be both numbers or strings");
    }
    return nullptr;
}

std::any lox::Interpreter::visit_call_expr(const std::shared_ptr<lox::CallExpr> expr) {
    std::any callee = evaluate(expr -> callee);
    std::shared_ptr<LoxCallable> function;
    if (callee.type() == typeid(std::shared_ptr<LoxFunction>))
        function = std::any_cast<std::shared_ptr<LoxFunction>>(callee);
    else if (callee.type() == typeid(std::shared_ptr<LoxClass>))
        function = std::any_cast<std::shared_ptr<LoxClass>>(callee);
    else if (callee.type() == typeid(std::shared_ptr<LoxCallable>))
        function = std::any_cast<std::shared_ptr<LoxCallable>>(callee);
    else
        throw RuntimeError(expr -> paren, "Can only call functions and methods");
    if (expr -> arguments -> size() != function -> arity())
        throw RuntimeError(expr -> paren, "Expected " + std::to_string(function -> arity()) + " arguments but got "
                                                      + std::to_string(expr -> arguments -> size()));
    std::shared_ptr<std::vector<std::any>> arguments = std::make_shared<std::vector<std::any>>();
    for (auto args : *expr -> arguments)
        arguments -> emplace_back(evaluate(args));
    return function -> call(*this, arguments);
}

std::any lox::Interpreter::visit_get_expr(const std::shared_ptr<lox::GetExpr> expr) {
    std::any object = evaluate(expr -> object);
    if (object.type() == typeid(std::shared_ptr<LoxInstance>))
        return std::any_cast<std::shared_ptr<LoxInstance>>(object) -> get(expr -> name);
    throw RuntimeError(expr -> name, "Only instances have properties.");
}

std::any lox::Interpreter::visit_grouping_expr(const std::shared_ptr<lox::GroupingExpr> expr) {
    return evaluate(expr -> expr);
}

std::any lox::Interpreter::visit_literal_expr(const std::shared_ptr<lox::LiteralExpr> expr) {
    return expr -> value;
}

std::any lox::Interpreter::visit_logical_expr(const std::shared_ptr<lox::LogicalExpr> expr) {
    std::any left = evaluate(expr -> left);
    if (expr -> op.type == OR) {
        if (is_truthy(left))
            return left;
    } else {
        if (!is_truthy(left))
            return left;
    }
    return evaluate(expr -> right);
}

std::any lox::Interpreter::visit_set_expr(const std::shared_ptr<lox::SetExpr> expr) {
    std::any object = evaluate(expr -> object);
    if (object.type() != typeid(std::shared_ptr<LoxInstance>))
        throw RuntimeError(expr -> name, "Only instances have fields");
    std::any value = evaluate(expr -> value);
    std::any_cast<std::shared_ptr<LoxInstance>>(object) -> set(expr -> name, value);
    return value;
}

std::any lox::Interpreter::visit_super_expr(const std::shared_ptr<lox::SuperExpr> expr) {
    const int distance = locals[expr];
    std::shared_ptr<LoxClass> superclass = std::any_cast<std::shared_ptr<LoxClass>>(environment -> get_at(distance, "super"));
    std::shared_ptr<LoxInstance> object = std::any_cast<std::shared_ptr<LoxInstance>>(environment -> get_at(distance - 1, "this"));
    std::shared_ptr<LoxFunction> method = superclass -> find_method(expr -> method.lexeme);
    if (method == nullptr)
        throw RuntimeError(expr -> method, "Undefined property '" + expr -> method.lexeme +"'");
    return method -> bind(object);
}

std::any lox::Interpreter::visit_this_expr(const std::shared_ptr<lox::ThisExpr> expr) {
    return lookup_variable(expr -> keyword, expr);
}

std::any lox::Interpreter::visit_unary_expr(const std::shared_ptr<lox::UnaryExpr> expr) {
    std::any right = evaluate(expr -> right);
    switch (expr -> op.type) {
        case BANG:
            return !is_truthy(right);
        case MINUS:
            check_number_operand(expr -> op, right);
            return -std::any_cast<double>(right);
    }
    return nullptr;
}

std::any lox::Interpreter::visit_variable_expr(const std::shared_ptr<lox::VariableExpr> expr) {
    return lookup_variable(expr -> name, expr);
}

void lox::Interpreter::visit_block_stmt(const std::shared_ptr<lox::BlockStmt> statement) {
    execute_block(statement -> statements, std::make_shared<Environment>(environment));
}

void lox::Interpreter::visit_class_stmt(const std::shared_ptr<lox::ClassStmt> statement) {
    std::any superclass = nullptr;
    std::shared_ptr<LoxClass> superclassptr = nullptr;
    if (statement -> superclass != nullptr) {
        superclass = evaluate(statement -> superclass);
        if (superclass.type() != typeid(std::shared_ptr<LoxClass>))
            throw RuntimeError(statement -> superclass -> name, "superclass must be a class");
        else
            superclassptr = std::any_cast<std::shared_ptr<LoxClass>>(superclass);
    }
    if (statement -> superclass != nullptr) {
        environment = std::make_shared<Environment>(environment);
        environment -> define("super", superclass);
    }
    const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<LoxFunction>>> methods = std::make_shared<std::unordered_map<std::string, std::shared_ptr<LoxFunction>>>(std::unordered_map<std::string, std::shared_ptr<LoxFunction>>());
    for (auto method : *statement -> methods) {
        (*methods)[method -> name.lexeme] = std::make_shared<LoxFunction>(method, environment, method -> name.lexeme == "init");
    }
    std::shared_ptr<LoxClass> klass = std::make_shared<LoxClass>(statement -> name.lexeme, methods, superclassptr);
    if (superclassptr != nullptr)
        environment = environment -> enclosing;
    environment -> define(statement -> name.lexeme, klass);
}

void lox::Interpreter::visit_fn_stmt(const std::shared_ptr<lox::FnStmt> statement) {
    std::shared_ptr<LoxFunction> function = std::make_shared<LoxFunction>(statement, environment, false);
    environment -> define(statement -> name.lexeme, function);
}

void lox::Interpreter::visit_if_stmt(const std::shared_ptr<lox::IfStmt> statement) {
    if (is_truthy(evaluate(statement -> condition)))
        execute(statement -> then);
    else if (statement -> otherwise != nullptr)
        execute(statement -> otherwise);
}

void lox::Interpreter::visit_print_stmt(const std::shared_ptr<lox::PrintStmt> statement) {
    std::any value = evaluate(statement -> expr);
    std::cout << stringfy(value) << "\n";
}

void lox::Interpreter::visit_expr_stmt(const std::shared_ptr<lox::ExprStmt> statement) {
    evaluate(statement -> expr);
}

void lox::Interpreter::visit_var_stmt(const std::shared_ptr<lox::VarStmt> statement) {
    std::any value = nullptr;
    if (statement -> initializer != nullptr)
        value = evaluate(statement -> initializer);
    environment -> define(statement -> name.lexeme, value);
}

void lox::Interpreter::visit_return_stmt(const std::shared_ptr<lox::ReturnStmt> statement) {
    throw Return(evaluate(statement -> value));
}

void lox::Interpreter::visit_while_stmt(const std::shared_ptr<lox::WhileStmt> statement) {
    while (is_truthy(evaluate(statement -> condition)))
        execute(statement -> body);
}

std::any lox::Interpreter::lookup_variable(const lox::Token name, const std::shared_ptr<lox::Expr> expr) {
    if (locals.contains(expr))
        return environment -> get_at(locals[expr], name.lexeme);
    return globals -> get(name);
}

void lox::Interpreter::interpret(const std::vector<std::shared_ptr<lox::Stmt>>& statements) {
    try {
        for (auto statement : statements)
            execute(statement);
    } catch (RuntimeError error) {
        runtime_error(error);
    }
}

void lox::Interpreter::resolve(const std::shared_ptr<Expr> expr, const int depth) {
    locals[expr] = depth;
}

std::string lox::Interpreter::stringfy(const std::any value) const {
    const auto& type = value.type();
    if (type == typeid(bool))
        return std::any_cast<bool>(value) ? "true" : "false";
    if (type == typeid(double))
        return std::to_string(std::any_cast<double>(value));
    if (type == typeid(std::string))
        return std::any_cast<std::string>(value);
    if (type == typeid(std::shared_ptr<LoxFunction>))
        return std::any_cast<std::shared_ptr<LoxFunction>>(value) -> to_string();
    if (type == typeid(std::shared_ptr<LoxClass>))
        return std::any_cast<std::shared_ptr<LoxClass>>(value) -> to_string();
    if (type == typeid(std::shared_ptr<LoxInstance>))
        return std::any_cast<std::shared_ptr<LoxInstance>>(value) -> to_string();
    if (type == typeid(std::shared_ptr<LoxCallable>))
        return std::any_cast<std::shared_ptr<LoxCallable>>(value) -> to_string();
    return "nil";
}
