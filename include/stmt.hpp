#ifndef STMT_HPP
#define STMT_HPP

#include "expression.hpp"


namespace lox {

struct PrintStmt;
struct ExprStmt;

struct StmtVisitor {
    virtual std::any visitPrintStmt(const std::shared_ptr<PrintStmt>) = 0;
    virtual std::any visitExprStmt(const std::shared_ptr<ExprStmt>) = 0;
};

struct Stmt {
    virtual std::any accept(StmtVisitor&) = 0;
};

struct PrintStmt : Stmt, public std::enable_shared_from_this<PrintStmt> {

    const std::shared_ptr<Expr> expr;

    PrintStmt(const std::shared_ptr<Expr> expr) : expr(expr) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitPrintStmt(shared_from_this());
    }

};

struct ExprStmt : Stmt, public std::enable_shared_from_this<ExprStmt> {

    const std::shared_ptr<Expr> expr;
    
    ExprStmt(const std::shared_ptr<Expr> expr) : expr(expr) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitExprStmt(shared_from_this());
    }

};

};

#endif
