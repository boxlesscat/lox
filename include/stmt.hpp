#ifndef STMT_HPP
#define STMT_HPP

#include "expression.hpp"

namespace lox {

struct BlockStmt;
struct ClassStmt;
struct FnStmt;
struct IfStmt;
struct VarStmt;
struct PrintStmt;
struct ExprStmt;
struct ReturnStmt;
struct WhileStmt;

struct StmtVisitor {
    virtual void visit(BlockStmt&)  = 0;
    virtual void visit(ClassStmt&)  = 0;
    virtual void visit(FnStmt&)     = 0;
    virtual void visit(IfStmt&)     = 0;
    virtual void visit(VarStmt&)    = 0;
    virtual void visit(PrintStmt&)  = 0;
    virtual void visit(ExprStmt&)   = 0;
    virtual void visit(ReturnStmt&) = 0;
    virtual void visit(WhileStmt&)  = 0;
};

struct Stmt {
    virtual void accept(StmtVisitor&) = 0;
};

struct BlockStmt : Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;

    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct ClassStmt : Stmt {
    std::vector<std::unique_ptr<FnStmt>> methods;
    Token                                name;
    std::unique_ptr<VariableExpr>        superclass;

    ClassStmt(Token name, std::vector<std::unique_ptr<FnStmt>> methods, std::unique_ptr<VariableExpr> superclass)
        : name(std::move(name)), methods(std::move(methods)), superclass(std::move(superclass)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct ExprStmt : Stmt {
    std::unique_ptr<Expr> expr;

    ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct FnStmt : Stmt {
    Token                              name;
    std::vector<Token>                 params;
    std::vector<std::unique_ptr<Stmt>> body;

    FnStmt(Token name, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body)
        : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct IfStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> then;
    std::unique_ptr<Stmt> otherwise;

    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then, std::unique_ptr<Stmt> otherwise)
        : condition(std::move(condition)), then(std::move(then)), otherwise(std::move(otherwise)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct PrintStmt : Stmt {
    std::unique_ptr<Expr> expr;

    PrintStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct ReturnStmt : Stmt {
    std::unique_ptr<Expr> value;
    Token                 keyword;

    ReturnStmt(Token keyword, std::unique_ptr<Expr> value) : keyword(std::move(keyword)), value(std::move(value)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct VarStmt : Stmt {
    Token                 name;
    std::unique_ptr<Expr> initializer;

    VarStmt(Token name, std::unique_ptr<Expr> initializer) : name(std::move(name)), initializer(std::move(initializer)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct WhileStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) : condition(std::move(condition)), body(std::move(body)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

}

#endif
