#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "vector"
#include "expression.hpp"
#include "stmt.hpp"


namespace lox {

class Interpreter : ExprVisitor, StmtVisitor {

    bool is_truthy(const std::any) const;
    bool is_equal(const std::any, const std::any) const;

    void check_number_operand(const Token, const std::any) const;
    void check_number_operands(const Token, const std::any, const std::any) const;

    void execute(const std::shared_ptr<Stmt>);  

    std::any evaluate(const std::shared_ptr<Expr>);

public:
    std::any visitBinaryExpr(const std::shared_ptr<BinaryExpr>) override;
    std::any visitGroupingExpr(const std::shared_ptr<GroupingExpr>) override;
    std::any visitLiteralExpr(const std::shared_ptr<LiteralExpr>) override;
    std::any visitUnaryExpr(const std::shared_ptr<UnaryExpr>) override;
    
    std::any visitPrintStmt(const std::shared_ptr<PrintStmt>) override;
    std::any visitExprStmt(const std::shared_ptr<ExprStmt>) override;

    void interpret(const std::vector<std::shared_ptr<Stmt>>&);
    std::string stringfy(const std::any) const;

};

};

#endif
