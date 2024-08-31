#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "environment.hpp"
#include "expression.hpp"
#include "stmt.hpp"

#include <vector>

namespace lox {

class Interpreter : ExprVisitor, StmtVisitor {

public:
    std::shared_ptr<Environment> globals = std::make_shared<Environment>();

private:
    std::shared_ptr<Environment>   environment = globals;
    std::unordered_map<Expr*, int> locals;

    bool is_truthy(const Value&) const;
    bool is_equal(const Value&, const Value&) const;

    void check_number_operand(const Token&, const Value&) const;
    void check_number_operands(const Token&, const Value&, const Value&) const;

    void execute(std::unique_ptr<Stmt>&);

    Value evaluate(Expr&);
    Value evaluate(std::unique_ptr<Expr>&);

    Value visit(AssignExpr&) override;
    Value visit(BinaryExpr&) override;
    Value visit(CallExpr&) override;
    Value visit(GetExpr&) override;
    Value visit(GroupingExpr&) override;
    Value visit(LiteralExpr&) override;
    Value visit(LogicalExpr&) override;
    Value visit(SetExpr&) override;
    Value visit(SuperExpr&) override;
    Value visit(ThisExpr&) override;
    Value visit(UnaryExpr&) override;
    Value visit(VariableExpr&) override;

    void visit(BlockStmt&) override;
    void visit(ClassStmt&) override;
    void visit(FnStmt&) override;
    void visit(IfStmt&) override;
    void visit(VarStmt&) override;
    void visit(PrintStmt&) override;
    void visit(ExprStmt&) override;
    void visit(ReturnStmt&) override;
    void visit(WhileStmt&) override;

    Value lookup_variable(const Token&, Expr*);

public:
    Interpreter();
    void        execute_block(std::vector<std::unique_ptr<Stmt>>&, std::shared_ptr<Environment>);
    void        interpret(std::vector<std::unique_ptr<Stmt>>&);
    void        resolve(Expr*, const int);
    std::string stringfy(const Value&) const;
};

};

#endif
