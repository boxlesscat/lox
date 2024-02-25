#ifndef RESOLVER_HPP
#define RESOLVER_HPP

#include <unordered_map>
#include "interpreter.hpp"


namespace lox {

class Resolver : public ExprVisitor, StmtVisitor {

    enum class FunctionType {
        FUNCTION,
        METHOD,
        NONE,
    };

    enum class ClassType {
        CLASS,
        NONE,
    };

    Interpreter& interpreter;
    std::vector<std::unordered_map<std::string, bool>> scopes;
    FunctionType current_function = FunctionType::NONE;
    ClassType current_class = ClassType::NONE;

    void resolve(const std::shared_ptr<Expr>);
    void resolve(const std::shared_ptr<Stmt>);
    void resolve_function(const std::shared_ptr<FnStmt>, const FunctionType);
    void resolve_local(const std::shared_ptr<Expr>, Token);

    void begin_scope();
    void end_scope();

    void declare(const Token);
    void define(const Token);

    void visit_block_stmt(const std::shared_ptr<BlockStmt>) override;
    void visit_class_stmt(const std::shared_ptr<ClassStmt>) override;
    void visit_expr_stmt(const std::shared_ptr<ExprStmt>) override;
    void visit_fn_stmt(const std::shared_ptr<FnStmt>) override;
    void visit_if_stmt(const std::shared_ptr<IfStmt>) override;
    void visit_print_stmt(const std::shared_ptr<PrintStmt>) override;
    void visit_return_stmt(const std::shared_ptr<ReturnStmt>) override;
    void visit_var_stmt(const std::shared_ptr<VarStmt>) override;
    void visit_while_stmt(const std::shared_ptr<WhileStmt>) override;

    std::any visit_assign_expr(const std::shared_ptr<AssignExpr>) override;
    std::any visit_binary_expr(const std::shared_ptr<BinaryExpr>) override;
    std::any visit_call_expr(const std::shared_ptr<CallExpr>) override;
    std::any visit_get_expr(const std::shared_ptr<GetExpr>) override;
    std::any visit_grouping_expr(const std::shared_ptr<GroupingExpr>) override;
    std::any visit_literal_expr(const std::shared_ptr<LiteralExpr>) override;
    std::any visit_logical_expr(const std::shared_ptr<LogicalExpr>) override;
    std::any visit_set_expr(const std::shared_ptr<SetExpr>) override;
    std::any visit_this_expr(const std::shared_ptr<ThisExpr>) override;
    std::any visit_unary_expr(const std::shared_ptr<UnaryExpr>) override;
    std::any visit_variable_expr(const std::shared_ptr<VariableExpr>) override;

public:

    Resolver(Interpreter& interpreter) : interpreter(interpreter) {}

    void resolve(const std::shared_ptr<std::vector<std::shared_ptr<Stmt>>>);

};

};

#endif
