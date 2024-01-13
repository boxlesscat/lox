#include "ast_printer.hpp"

std::string lox::ASTPrinter::print(const std::shared_ptr<Expr> expr) {
    return std::any_cast<std::string>(expr -> accept(*this));
}

std::any lox::ASTPrinter::visit_binary_expr(const std::shared_ptr<lox::BinaryExpr> expr) {
    return parenthesize(expr -> op.lexeme,  {expr -> left, expr -> right});
}

std::any lox::ASTPrinter::visit_grouping_expr(const std::shared_ptr<lox::GroupingExpr> expr) {
    return parenthesize("group", {expr -> expr});
}

std::any lox::ASTPrinter::visit_literal_expr(const std::shared_ptr<lox::LiteralExpr> expr) {
    auto& type = expr -> value.type();
    if (type == typeid(bool))
        return std::any_cast<bool>(expr -> value) ? "true" : "false";
    if (type == typeid(double))
        return std::to_string(std::any_cast<double>(expr -> value));
    if (type == typeid(std::string))
        return std::any_cast<std::string>(expr -> value);
    return "nil";
}

std::any lox::ASTPrinter::visit_unary_expr(const std::shared_ptr<lox::UnaryExpr> expr) {
    return parenthesize(expr -> op.lexeme,  {expr -> right});
}
