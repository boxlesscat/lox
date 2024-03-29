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
    virtual void visit_block_stmt(const std::shared_ptr<BlockStmt>) = 0;
    virtual void visit_class_stmt(const std::shared_ptr<ClassStmt>) = 0;
    virtual void visit_fn_stmt(const std::shared_ptr<FnStmt>) = 0;
    virtual void visit_if_stmt(const std::shared_ptr<IfStmt>) = 0;
    virtual void visit_var_stmt(const std::shared_ptr<VarStmt>) = 0;
    virtual void visit_print_stmt(const std::shared_ptr<PrintStmt>) = 0;
    virtual void visit_expr_stmt(const std::shared_ptr<ExprStmt>) = 0;
    virtual void visit_return_stmt(const std::shared_ptr<ReturnStmt>) = 0;
    virtual void visit_while_stmt(const std::shared_ptr<WhileStmt>) = 0;
};

struct Stmt {
    virtual void accept(StmtVisitor&) = 0;
};

struct BlockStmt : Stmt, public std::enable_shared_from_this<BlockStmt> {

    const std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> statements;

    BlockStmt(const std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> statements) : statements(statements) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit_block_stmt(shared_from_this());
    }

};

struct ClassStmt : Stmt, public std::enable_shared_from_this<ClassStmt> {

    const std::shared_ptr<std::vector<std::shared_ptr<FnStmt>>> methods;
    const Token name;
    const std::shared_ptr<VariableExpr> superclass;

    ClassStmt(const Token name, const std::shared_ptr<std::vector<std::shared_ptr<FnStmt>>> methods, const std::shared_ptr<VariableExpr> superclass)
        : name(name), methods(methods), superclass(superclass) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit_class_stmt(shared_from_this());
    }

};

struct FnStmt : Stmt, public std::enable_shared_from_this<FnStmt> {

    const Token name;
    const std::shared_ptr<std::vector<Token>> params;
    const std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> body;

    FnStmt(const Token name, const std::shared_ptr<std::vector<Token>> params, const std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> body) :
        name(name), params(params), body(body) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit_fn_stmt(shared_from_this());
    }

};

struct IfStmt : Stmt, public std::enable_shared_from_this<IfStmt> {

    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> then;
    const std::shared_ptr<Stmt> otherwise;

    IfStmt(const std::shared_ptr<Expr> condition, const std::shared_ptr<Stmt> then, const std::shared_ptr<Stmt> otherwise) :
        condition(condition), then(then), otherwise(otherwise) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit_if_stmt(shared_from_this());
    }

};

struct VarStmt : Stmt, public std::enable_shared_from_this<VarStmt> {
    
    const Token name;
    const std::shared_ptr<Expr> initializer;

    VarStmt(const Token name, const std::shared_ptr<Expr> initializer) : name(name), initializer(initializer) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit_var_stmt(shared_from_this());
    }

};

struct PrintStmt : Stmt, public std::enable_shared_from_this<PrintStmt> {

    const std::shared_ptr<Expr> expr;

    PrintStmt(const std::shared_ptr<Expr> expr) : expr(expr) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit_print_stmt(shared_from_this());
    }

};

struct ExprStmt : Stmt, public std::enable_shared_from_this<ExprStmt> {

    const std::shared_ptr<Expr> expr;
    
    ExprStmt(const std::shared_ptr<Expr> expr) : expr(expr) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visit_expr_stmt(shared_from_this());
    }

};

struct ReturnStmt : Stmt, public std::enable_shared_from_this<ReturnStmt> {

    const std::shared_ptr<Expr> value;
    const Token keyword;

    ReturnStmt(const Token keyword, const std::shared_ptr<Expr> value) : keyword(keyword), value(value) {}

    void accept(StmtVisitor& visitor) {
        visitor.visit_return_stmt(shared_from_this());
    }

};

struct WhileStmt : Stmt, public std::enable_shared_from_this<WhileStmt> {

    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> body;

    WhileStmt(const std::shared_ptr<Expr> condition, const std::shared_ptr<Stmt> body) :
        condition(condition), body(body) {}
    
    void accept(StmtVisitor& visitor) override {
        visitor.visit_while_stmt(shared_from_this());
    }

};

};

#endif
