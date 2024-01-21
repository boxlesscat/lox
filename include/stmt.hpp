#ifndef STMT_HPP
#define STMT_HPP

#include "expression.hpp"


namespace lox {

struct BlockStmt;
struct VarStmt;
struct PrintStmt;
struct ExprStmt;

struct StmtVisitor {
    virtual void visit_block_stmt(const std::shared_ptr<BlockStmt>) = 0;
    virtual void visit_var_stmt(const std::shared_ptr<VarStmt>) = 0;
    virtual void visit_print_stmt(const std::shared_ptr<PrintStmt>) = 0;
    virtual void visit_expr_stmt(const std::shared_ptr<ExprStmt>) = 0;
};

struct Stmt {
    virtual void accept(StmtVisitor&) = 0;
};

struct BlockStmt : Stmt, public std::enable_shared_from_this<BlockStmt> {

    const std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> statements;

    BlockStmt(const std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> statements) : statements(statements) {}

    void accept(StmtVisitor& visitor) override {
        return visitor.visit_block_stmt(shared_from_this());
    }

};

struct VarStmt : Stmt, public std::enable_shared_from_this<VarStmt> {
    
    const Token name;
    const std::shared_ptr<Expr> initializer;

    VarStmt(const Token name, const std::shared_ptr<Expr> initializer) : name(name), initializer(initializer) {}

    void accept(StmtVisitor& visitor) override {
        return visitor.visit_var_stmt(shared_from_this());
    }

};

struct PrintStmt : Stmt, public std::enable_shared_from_this<PrintStmt> {

    const std::shared_ptr<Expr> expr;

    PrintStmt(const std::shared_ptr<Expr> expr) : expr(expr) {}

    void accept(StmtVisitor& visitor) override {
        return visitor.visit_print_stmt(shared_from_this());
    }

};

struct ExprStmt : Stmt, public std::enable_shared_from_this<ExprStmt> {

    const std::shared_ptr<Expr> expr;
    
    ExprStmt(const std::shared_ptr<Expr> expr) : expr(expr) {}

    void accept(StmtVisitor& visitor) override {
        return visitor.visit_expr_stmt(shared_from_this());
    }

};

};

#endif
