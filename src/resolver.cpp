#include "error.hpp"
#include "resolver.hpp"


void lox::Resolver::resolve(const std::shared_ptr<lox::Expr> expr) {
    expr -> accept(*this);
}

void lox::Resolver::resolve(const std::shared_ptr<lox::Stmt> stmt) {
    stmt -> accept(*this);
}

void lox::Resolver::resolve_function(const std::shared_ptr<lox::FnStmt> stmt, const lox::Resolver::FunctionType type) {
    FunctionType enclosing_function = current_function;
    current_function = type;
    begin_scope();
    for (Token param : *stmt -> params) {
        declare(param);
        define(param);
    }
    resolve(stmt -> body);
    end_scope();
    current_function = enclosing_function;
}

void lox::Resolver::resolve_local(const std::shared_ptr<lox::Expr> expr, Token name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes[i].contains(name.lexeme)) {
            interpreter.resolve(expr, scopes.size() - i - 1);
            return;
        }
    }
}

void lox::Resolver::begin_scope() {
    scopes.emplace_back(std::unordered_map<std::string, bool>());
}

void lox::Resolver::end_scope() {
    scopes.pop_back();
}

void lox::Resolver::declare(const lox::Token name) {
    if (!scopes.empty())
        scopes.back()[name.lexeme] = false;
}

void lox::Resolver::define(const lox::Token name) {
    if (!scopes.empty())
        scopes.back()[name.lexeme] = true;
}

void lox::Resolver::visit_block_stmt(const std::shared_ptr<lox::BlockStmt> stmt) {
    begin_scope();
    resolve(stmt -> statements);
    end_scope();
}

void lox::Resolver::visit_class_stmt(const std::shared_ptr<lox::ClassStmt> stmt) {
    declare(stmt -> name);
    define(stmt -> name);
}

void lox::Resolver::visit_expr_stmt(const std::shared_ptr<lox::ExprStmt> stmt) {
    resolve(stmt -> expr);
}

void lox::Resolver::visit_fn_stmt(const std::shared_ptr<lox::FnStmt> stmt) {
    declare(stmt -> name);
    define(stmt -> name);
    resolve_function(stmt, FunctionType::FUNCTION);
}

void lox::Resolver::visit_if_stmt(const std::shared_ptr<lox::IfStmt> stmt) {
    resolve(stmt -> condition);
    resolve(stmt -> then);
    if (stmt -> otherwise != nullptr)
        resolve(stmt -> otherwise);
}

void lox::Resolver::visit_print_stmt(const std::shared_ptr<lox::PrintStmt> stmt) {
    resolve(stmt -> expr);
}

void lox::Resolver::visit_return_stmt(const std::shared_ptr<lox::ReturnStmt> stmt) {
    if (current_function == FunctionType::NONE)
        error(stmt -> keyword, "Can't return from top level code");
    if (stmt -> value != nullptr)
        resolve(stmt -> value);
}

void lox::Resolver::visit_var_stmt(const std::shared_ptr<lox::VarStmt> stmt) {
    declare(stmt -> name);
    if (stmt -> initializer != nullptr)
        resolve(stmt -> initializer);
    define(stmt -> name);
}

void lox::Resolver::visit_while_stmt(const std::shared_ptr<lox::WhileStmt> stmt) {
    resolve(stmt -> condition);
    resolve(stmt -> body);
}

std::any lox::Resolver::visit_assign_expr(const std::shared_ptr<lox::AssignExpr> expr) {
    resolve(expr -> value);
    resolve_local(expr, expr -> name);
    return nullptr;
}

std::any lox::Resolver::visit_binary_expr(const std::shared_ptr<lox::BinaryExpr> expr) {
    resolve(expr -> left);
    resolve(expr -> right);
    return nullptr;
}

std::any lox::Resolver::visit_call_expr(const std::shared_ptr<lox::CallExpr> expr) {
    resolve(expr -> callee);
    for (auto arg : *expr -> arguments)
        resolve(arg);
    return nullptr;
}

std::any lox::Resolver::visit_get_expr(const std::shared_ptr<lox::GetExpr> expr) {
    resolve(expr -> object);
    return nullptr;
}

std::any lox::Resolver::visit_grouping_expr(const std::shared_ptr<lox::GroupingExpr> expr) {
    resolve(expr -> expr);
    return nullptr;
}

std::any lox::Resolver::visit_literal_expr(const std::shared_ptr<lox::LiteralExpr> expr) {
    return nullptr;
}

std::any lox::Resolver::visit_logical_expr(const std::shared_ptr<lox::LogicalExpr> expr) {
    resolve(expr -> left);
    resolve(expr -> right);
    return nullptr;
}

std::any lox::Resolver::visit_set_expr(const std::shared_ptr<SetExpr> expr) {
    resolve(expr -> object);
    resolve(expr -> value);
    return nullptr;
}

std::any lox::Resolver::visit_unary_expr(const std::shared_ptr<lox::UnaryExpr> expr) {
    resolve(expr -> right);
    return nullptr;
}

std::any lox::Resolver::visit_variable_expr(const std::shared_ptr<lox::VariableExpr> expr) {
    if (!scopes.empty() and scopes.back().contains(expr -> name.lexeme) and !scopes.back()[expr -> name.lexeme])
        error(expr -> name, "Can't read local variable in its own initializer");
    resolve_local(expr, expr -> name);
    return nullptr;
}

void lox::Resolver::resolve(const std::shared_ptr<std::vector<std::shared_ptr<lox::Stmt>>> statements) {
    for (auto statement : *statements)
        resolve(statement);
}
