#include "interpreter.hpp"
#include "error.hpp"


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

std::any lox::Interpreter::evaluate(const std::shared_ptr<lox::Expr> expr) {
    return expr -> accept(*this);
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

std::any lox::Interpreter::visit_grouping_expr(const std::shared_ptr<lox::GroupingExpr> expr) {
    return evaluate(expr -> expr);
}

std::any lox::Interpreter::visit_literal_expr(const std::shared_ptr<lox::LiteralExpr> expr) {
    return expr -> value;
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

std::any lox::Interpreter::visit_print_stmt(const std::shared_ptr<lox::PrintStmt> statement) {
    std::any value = evaluate(statement -> expr);
    std::cout << stringfy(value) << "\n";
    return nullptr;
}

std::any lox::Interpreter::visit_expr_stmt(const std::shared_ptr<lox::ExprStmt> statement) {
    evaluate(statement -> expr);
    return nullptr;
}

void lox::Interpreter::interpret(const std::vector<std::shared_ptr<lox::Stmt>>& statements) {
    try {
        for (auto statement : statements)
            execute(statement);
    } catch (RuntimeError error) {
        runtime_error(error);
    }
}

std::string lox::Interpreter::stringfy(const std::any value) const {
    const auto& type = value.type();
    if (type == typeid(bool))
        return std::any_cast<bool>(value) ? "true" : "false";
    if (type == typeid(double))
        return std::to_string(std::any_cast<double>(value));
    if (type == typeid(std::string))
        return std::any_cast<std::string>(value);
    return "nil";
}
