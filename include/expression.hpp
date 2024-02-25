#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include "token.hpp"
#include <vector>


namespace lox {

struct AssignExpr;
struct BinaryExpr;
struct CallExpr;
struct GetExpr;
struct GroupingExpr;
struct LiteralExpr;
struct LogicalExpr;
struct SetExpr;
struct UnaryExpr;
struct VariableExpr;

struct ExprVisitor {
    virtual std::any visit_assign_expr(const std::shared_ptr<AssignExpr>) = 0;
    virtual std::any visit_binary_expr(const std::shared_ptr<BinaryExpr>) = 0;
    virtual std::any visit_call_expr(const std::shared_ptr<CallExpr>) = 0;
    virtual std::any visit_get_expr(const std::shared_ptr<GetExpr>) = 0;
    virtual std::any visit_grouping_expr(const std::shared_ptr<GroupingExpr>) = 0;
    virtual std::any visit_literal_expr(const std::shared_ptr<LiteralExpr>) = 0;
    virtual std::any visit_logical_expr(const std::shared_ptr<LogicalExpr>) = 0;
    virtual std::any visit_set_expr(const std::shared_ptr<SetExpr>) = 0;
    virtual std::any visit_unary_expr(const std::shared_ptr<UnaryExpr>) = 0;
    virtual std::any visit_variable_expr(const std::shared_ptr<VariableExpr>) = 0;
};

struct Expr {
    virtual std::any accept(ExprVisitor&) = 0;
};

struct AssignExpr : Expr, public std::enable_shared_from_this<AssignExpr> {

    const Token name;
    const std::shared_ptr<Expr> value;

    AssignExpr(Token name, const std::shared_ptr<Expr> value) : name(std::move(name)), value(value) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_assign_expr(shared_from_this());
    }

};

struct BinaryExpr : Expr, public std::enable_shared_from_this<BinaryExpr> {
    
    const std::shared_ptr<Expr> left;
    const Token op; // operator
    const std::shared_ptr<Expr> right;
    
    BinaryExpr(const std::shared_ptr<Expr> left, Token token, const std::shared_ptr<Expr> right) :
        left(std::move(left)), op(std::move(token)), right(std::move(right)) {}
    
    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_binary_expr(shared_from_this());
    }

};

struct CallExpr : Expr, public std::enable_shared_from_this<CallExpr> {

    const std::shared_ptr<Expr> callee;
    const Token paren;
    const std::shared_ptr<std::vector<std::shared_ptr<Expr>>> arguments;

    CallExpr(const std::shared_ptr<Expr> callee, Token paren, const std::shared_ptr<std::vector<std::shared_ptr<Expr>>> arguments) :
        callee(callee), paren(paren), arguments(arguments) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_call_expr(shared_from_this());
    }

};

struct GetExpr : Expr, public std::enable_shared_from_this<GetExpr> {

    const std::shared_ptr<Expr> object;
    const Token name;

    GetExpr(const std::shared_ptr<Expr> object, const Token name) : object(object), name(name) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_get_expr(shared_from_this());
    }

};

struct GroupingExpr : Expr, public std::enable_shared_from_this<GroupingExpr> {
    
    const std::shared_ptr<Expr> expr;
    
    GroupingExpr(const std::shared_ptr<Expr> expr) :
        expr(std::move(expr)) {}
    
    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_grouping_expr(shared_from_this());
    }

};

struct LiteralExpr : Expr, public std::enable_shared_from_this<LiteralExpr> {
    
    const std::any value;
    
    LiteralExpr(const std::any value) :
        value(std::move(value)) {}
    
    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_literal_expr(shared_from_this());
    }

};

struct LogicalExpr : Expr, public std::enable_shared_from_this<LogicalExpr> {

    const std::shared_ptr<Expr> left;
    const Token op; // operator
    const std::shared_ptr<Expr> right;

    LogicalExpr(const std::shared_ptr<Expr> left, Token op, const std::shared_ptr<Expr> right) :
        left(left), op(op), right(right) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_logical_expr(shared_from_this());
    }

};

struct SetExpr : Expr, public std::enable_shared_from_this<SetExpr> {

    const std::shared_ptr<Expr> object;
    const std::shared_ptr<Expr> value;
    const Token name;

    SetExpr(const std::shared_ptr<Expr> object, const std::shared_ptr<Expr> value, const Token name) :
        object(object), value(value), name(name) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_set_expr(shared_from_this());
    }

};

struct UnaryExpr : Expr, public std::enable_shared_from_this<UnaryExpr> {
    
    const Token op; // operator
    const std::shared_ptr<Expr> right;
    
    UnaryExpr(Token token, const std::shared_ptr<Expr> right) :
        op(std::move(token)), right(std::move(right)) {}
    
    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_unary_expr(shared_from_this());
    }

};

struct VariableExpr : Expr, public std::enable_shared_from_this<VariableExpr> {

    const Token name;

    VariableExpr(Token name) : name(std::move(name)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_variable_expr(shared_from_this());
    }

};

};

#endif
