#include "resolver.hpp"

#include "error.hpp"

void lox::Resolver::resolve(const std::unique_ptr<lox::VariableExpr>& expr) {
    expr->accept(*this);
}

void lox::Resolver::resolve(const std::unique_ptr<lox::Expr>& expr) {
    expr->accept(*this);
}

void lox::Resolver::resolve(const std::unique_ptr<lox::Stmt>& stmt) {
    stmt->accept(*this);
}

void lox::Resolver::resolve_function(const lox::FnStmt& stmt, const lox::Resolver::FunctionType type) {
    FunctionType enclosing_function = current_function;
    current_function                = type;
    begin_scope();
    for (const Token& param : stmt.params) {
        declare(param);
        define(param);
    }
    resolve(stmt.body);
    end_scope();
    current_function = enclosing_function;
}

void lox::Resolver::resolve_local(lox::Expr& expr, const Token& name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes[i].contains(name.lexeme)) {
            interpreter.resolve(&expr, scopes.size() - i - 1);
            return;
        }
    }
}

void lox::Resolver::begin_scope() {
    scopes.emplace_back();
}

void lox::Resolver::end_scope() {
    scopes.pop_back();
}

void lox::Resolver::declare(const lox::Token& name) {
    if (!scopes.empty())
        scopes.back()[name.lexeme] = false;
}

void lox::Resolver::define(const lox::Token& name) {
    if (!scopes.empty())
        scopes.back()[name.lexeme] = true;
}

void lox::Resolver::visit(lox::BlockStmt& stmt) {
    begin_scope();
    resolve(stmt.statements);
    end_scope();
}

void lox::Resolver::visit(lox::ClassStmt& stmt) {
    ClassType enclosing_class = current_class;
    current_class             = ClassType::CLASS;
    declare(stmt.name);
    define(stmt.name);
    if (stmt.superclass)
        if (stmt.name.lexeme == stmt.superclass->name.lexeme)
            error(stmt.superclass->name, "A class cannot inherit from itself");
        else {
            current_class = ClassType::SUBCLASS;
            resolve(stmt.superclass);
        }
    if (stmt.superclass) {
        begin_scope();
        scopes.back()["super"] = true;
    }
    begin_scope();
    scopes.back()["this"] = true;
    FunctionType declaration;
    for (const auto& method : stmt.methods) {
        if (method->name.lexeme == "init")
            declaration = FunctionType::INITIALIZER;
        else
            declaration = FunctionType::METHOD;
        resolve_function(*method.get(), declaration);
    }
    if (stmt.superclass)
        end_scope();
    end_scope();
    current_class = enclosing_class;
}

void lox::Resolver::visit(lox::ExprStmt& stmt) {
    resolve(stmt.expr);
}

void lox::Resolver::visit(lox::FnStmt& stmt) {
    declare(stmt.name);
    define(stmt.name);
    resolve_function(stmt, FunctionType::FUNCTION);
}

void lox::Resolver::visit(lox::IfStmt& stmt) {
    resolve(stmt.condition);
    resolve(stmt.then);
    if (stmt.otherwise)
        resolve(stmt.otherwise);
}

void lox::Resolver::visit(lox::PrintStmt& stmt) {
    resolve(stmt.expr);
}

void lox::Resolver::visit(lox::ReturnStmt& stmt) {
    if (current_function == FunctionType::NONE)
        error(stmt.keyword, "Can't return from top level code");
    if (stmt.value) {
        if (current_function == FunctionType::INITIALIZER)
            error(stmt.keyword, "Can't return a value from initializer");
        resolve(stmt.value);
    }
}

void lox::Resolver::visit(lox::VarStmt& stmt) {
    declare(stmt.name);
    if (stmt.initializer)
        resolve(stmt.initializer);
    define(stmt.name);
}

void lox::Resolver::visit(lox::WhileStmt& stmt) {
    resolve(stmt.condition);
    resolve(stmt.body);
}

lox::Value lox::Resolver::visit(lox::AssignExpr& expr) {
    resolve(expr.value);
    resolve_local(expr, expr.name);
    return {};
}

lox::Value lox::Resolver::visit(lox::BinaryExpr& expr) {
    resolve(expr.left);
    resolve(expr.right);
    return {};
}

lox::Value lox::Resolver::visit(lox::CallExpr& expr) {
    resolve(expr.callee);
    for (const auto& arg : expr.arguments)
        resolve(arg);
    return {};
}

lox::Value lox::Resolver::visit(lox::GetExpr& expr) {
    resolve(expr.object);
    return {};
}

lox::Value lox::Resolver::visit(lox::GroupingExpr& expr) {
    resolve(expr.expr);
    return {};
}

lox::Value lox::Resolver::visit(lox::LiteralExpr& expr) {
    // it's supposed to be empty
    return {};
}

lox::Value lox::Resolver::visit(lox::LogicalExpr& expr) {
    resolve(expr.left);
    resolve(expr.right);
    return {};
}

lox::Value lox::Resolver::visit(SetExpr& expr) {
    resolve(expr.object);
    resolve(expr.value);
    return {};
}

lox::Value lox::Resolver::visit(SuperExpr& expr) {
    if (current_class == ClassType::NONE)
        error(expr.keyword, "Can't use 'super' outside of a class");
    else if (current_class != ClassType::SUBCLASS)
        error(expr.keyword, "Can't use 'super' in class with no subclass");
    resolve_local(expr, expr.keyword);
    return {};
}

lox::Value lox::Resolver::visit(ThisExpr& expr) {
    if (current_class == ClassType::NONE)
        error(expr.keyword, "Can't use this outside of a class");
    resolve_local(expr, expr.keyword);
    return {};
}

lox::Value lox::Resolver::visit(lox::UnaryExpr& expr) {
    resolve(expr.right);
    return {};
}

lox::Value lox::Resolver::visit(lox::VariableExpr& expr) {
    if (!scopes.empty() and scopes.back().contains(expr.name.lexeme) and !scopes.back()[expr.name.lexeme])
        error(expr.name, "Can't read local variable in its own initializer");
    resolve_local(expr, expr.name);
    return {};
}

void lox::Resolver::resolve(const std::vector<std::unique_ptr<lox::Stmt>>& statements) {
    for (const auto& statement : statements)
        resolve(statement);
}
