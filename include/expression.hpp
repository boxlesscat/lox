#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "token.hpp"

#include <memory>
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
struct SuperExpr;
struct ThisExpr;
struct UnaryExpr;
struct VariableExpr;

struct ExprVisitor {
    virtual Value visit(AssignExpr&)   = 0;
    virtual Value visit(BinaryExpr&)   = 0;
    virtual Value visit(CallExpr&)     = 0;
    virtual Value visit(GetExpr&)      = 0;
    virtual Value visit(GroupingExpr&) = 0;
    virtual Value visit(LiteralExpr&)  = 0;
    virtual Value visit(LogicalExpr&)  = 0;
    virtual Value visit(SetExpr&)      = 0;
    virtual Value visit(SuperExpr&)    = 0;
    virtual Value visit(ThisExpr&)     = 0;
    virtual Value visit(UnaryExpr&)    = 0;
    virtual Value visit(VariableExpr&) = 0;
};

struct Expr {
    virtual Value accept(ExprVisitor&) = 0;
};

struct AssignExpr : Expr {
    Token                 name;
    std::unique_ptr<Expr> value;

    AssignExpr(Token name, std::unique_ptr<Expr> value) : name(std::move(name)), value(std::move(value)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct BinaryExpr : Expr {
    std::unique_ptr<Expr> left;
    Token                 op;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> left, Token token, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(token)), right(std::move(right)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct CallExpr : Expr {
    std::unique_ptr<Expr>              callee;
    Token                              paren;
    std::vector<std::unique_ptr<Expr>> arguments;

    CallExpr(std::unique_ptr<Expr> callee, Token paren, std::vector<std::unique_ptr<Expr>> arguments)
        : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct GetExpr : Expr {
    std::unique_ptr<Expr> object;
    Token                 name;

    GetExpr(std::unique_ptr<Expr> object, Token name) : object(std::move(object)), name(std::move(name)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct GroupingExpr : Expr {
    std::unique_ptr<Expr> expr;

    GroupingExpr(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct LiteralExpr : Expr {
    Value value;

    LiteralExpr(Value value) : value(std::move(value)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct LogicalExpr : Expr {
    std::unique_ptr<Expr> left;
    Token                 op;
    std::unique_ptr<Expr> right;

    LogicalExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct SetExpr : Expr {
    std::unique_ptr<Expr> object;
    std::unique_ptr<Expr> value;
    Token                 name;

    SetExpr(std::unique_ptr<Expr> object, std::unique_ptr<Expr> value, Token name)
        : object(std::move(object)), value(std::move(value)), name(std::move(name)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct SuperExpr : Expr {
    Token keyword;
    Token method;

    SuperExpr(Token keyword, Token method) : keyword(std::move(keyword)), method(std::move(method)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct ThisExpr : Expr {
    Token keyword;

    ThisExpr(Token keyword) : keyword(std::move(keyword)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct UnaryExpr : Expr {
    Token                 op;
    std::unique_ptr<Expr> right;

    UnaryExpr(Token token, std::unique_ptr<Expr> right) : op(std::move(token)), right(std::move(right)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

struct VariableExpr : Expr {
    Token name;

    VariableExpr(Token name) : name(std::move(name)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

}

#endif
