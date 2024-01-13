#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include "token.hpp"


namespace lox {

struct BinaryExpr;
struct GroupingExpr;
struct LiteralExpr;
struct UnaryExpr;

struct ExprVisitor {
    virtual std::any visit_binary_expr(const std::shared_ptr<BinaryExpr>) = 0;
    virtual std::any visit_grouping_expr(const std::shared_ptr<GroupingExpr>) = 0;
    virtual std::any visit_literal_expr(const std::shared_ptr<LiteralExpr>) = 0;
    virtual std::any visit_unary_expr(const std::shared_ptr<UnaryExpr>) = 0;
};

struct Expr {
    virtual std::any accept(ExprVisitor&) = 0;
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

struct UnaryExpr : Expr, public std::enable_shared_from_this<UnaryExpr> {
    
    const Token op; // operator
    const std::shared_ptr<Expr> right;
    
    UnaryExpr(Token token, const std::shared_ptr<Expr> right) :
        op(std::move(token)), right(std::move(right)) {}
    
    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_unary_expr(shared_from_this());
    }

};

};

#endif
