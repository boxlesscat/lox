#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include "token.hpp"


namespace lox {

struct BinaryExpr;
struct GroupingExpr;
struct LiteralExpr;
struct UnaryExpr;

struct Visitor {
    virtual std::any visitBinaryExpr(const std::shared_ptr<BinaryExpr> expr) = 0;
    virtual std::any visitGroupingExpr(const std::shared_ptr<GroupingExpr> expr) = 0;
    virtual std::any visitLiteralExpr(const std::shared_ptr<LiteralExpr> expr) = 0;
    virtual std::any visitUnaryExpr(const std::shared_ptr<UnaryExpr> expr) = 0;
};

struct Expr {
    virtual std::any accept(Visitor& visitor) = 0;
};

struct BinaryExpr : Expr, public std::enable_shared_from_this<BinaryExpr> {
    
    const std::shared_ptr<Expr> left;
    const Token token;
    const std::shared_ptr<Expr> right;
    
    BinaryExpr(const std::shared_ptr<Expr> left, Token token, const std::shared_ptr<Expr> right) :
        left(std::move(left)), token(std::move(token)), right(std::move(right)) {}
    
    std::any accept(Visitor& visitor) override {
        visitor.visitBinaryExpr(shared_from_this());
    }

};

struct GroupingExpr : Expr, public std::enable_shared_from_this<GroupingExpr> {
    
    const std::shared_ptr<Expr> expr;
    
    GroupingExpr(const std::shared_ptr<Expr> expr) :
        expr(std::move(expr)) {}
    
    std::any accept(Visitor& visitor) override {
        visitor.visitGroupingExpr(shared_from_this());
    }

};

struct LiteralExpr : Expr, public std::enable_shared_from_this<LiteralExpr> {
    
    const std::shared_ptr<std::any> value;
    
    LiteralExpr(const std::shared_ptr<std::any> value) :
        value(std::move(value)) {}
    
    std::any accept(Visitor& visitor) override {
        visitor.visitLiteralExpr(shared_from_this());
    }

};

struct UnaryExpr : Expr, public std::enable_shared_from_this<UnaryExpr> {
    
    const Token token;
    const std::shared_ptr<Expr> right;
    
    UnaryExpr(Token token, const std::shared_ptr<Expr> right) :
        token(std::move(token)), right(std::move(right)) {}
    
    std::any accept(Visitor& visitor) override {
        visitor.visitUnaryExpr(shared_from_this());
    }

};

};

#endif
