#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "vector"
#include "environment.hpp"
#include "expression.hpp"
#include "loxclass.hpp"
#include "stmt.hpp"


namespace lox {

class Interpreter : ExprVisitor, StmtVisitor {

public:
    std::shared_ptr<Environment> globals = std::make_shared<Environment>();
private:
    std::shared_ptr<Environment> environment = globals;
    std::unordered_map<std::shared_ptr<Expr>, int> locals;

    bool is_truthy(const std::any) const;
    bool is_equal(const std::any, const std::any) const;

    void check_number_operand(const Token, const std::any) const;
    void check_number_operands(const Token, const std::any, const std::any) const;

    void execute(const std::shared_ptr<Stmt>);
    
    std::any evaluate(const std::shared_ptr<Expr>);

    std::any visit_assign_expr(const std::shared_ptr<AssignExpr>) override;
    std::any visit_binary_expr(const std::shared_ptr<BinaryExpr>) override;
    std::any visit_call_expr(const std::shared_ptr<CallExpr>) override;
    std::any visit_grouping_expr(const std::shared_ptr<GroupingExpr>) override;
    std::any visit_literal_expr(const std::shared_ptr<LiteralExpr>) override;
    std::any visit_logical_expr(const std::shared_ptr<LogicalExpr>) override;
    std::any visit_unary_expr(const std::shared_ptr<UnaryExpr>) override;
    std::any visit_variable_expr(const std::shared_ptr<VariableExpr>) override;
    
    void visit_block_stmt(const std::shared_ptr<BlockStmt>) override;
    void visit_class_stmt(const std::shared_ptr<ClassStmt>) override;
    void visit_fn_stmt(const std::shared_ptr<FnStmt>) override;
    void visit_if_stmt(const std::shared_ptr<IfStmt>) override;
    void visit_var_stmt(const std::shared_ptr<VarStmt>) override;
    void visit_print_stmt(const std::shared_ptr<PrintStmt>) override;
    void visit_expr_stmt(const std::shared_ptr<ExprStmt>) override;
    void visit_return_stmt(const std::shared_ptr<ReturnStmt>) override;
    void visit_while_stmt(const std::shared_ptr<WhileStmt>) override;

    std::any lookup_variable(const Token, const std::shared_ptr<Expr>);

public:
    Interpreter();
    void execute_block(const std::shared_ptr<std::vector<std::shared_ptr<Stmt>>>, std::shared_ptr<Environment>);
    void interpret(const std::vector<std::shared_ptr<Stmt>>&);
    void resolve(const std::shared_ptr<Expr>, const int);
    std::string stringfy(const std::any) const;

};

};

#endif
