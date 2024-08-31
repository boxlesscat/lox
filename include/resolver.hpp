#ifndef RESOLVER_HPP
#define RESOLVER_HPP

#include "interpreter.hpp"

#include <unordered_map>

namespace lox {

class Resolver : public ExprVisitor, StmtVisitor {

    enum class FunctionType {
        FUNCTION,
        INITIALIZER,
        METHOD,
        NONE,
    };

    enum class ClassType {
        CLASS,
        NONE,
        SUBCLASS,
    };

    Interpreter&                                       interpreter;
    std::vector<std::unordered_map<std::string, bool>> scopes;
    FunctionType                                       current_function = FunctionType::NONE;
    ClassType                                          current_class    = ClassType::NONE;

    void resolve(const std::unique_ptr<Expr>&);
    void resolve(const std::unique_ptr<VariableExpr>&);
    void resolve(const std::unique_ptr<Stmt>&);
    void resolve_function(const FnStmt&, const FunctionType);
    void resolve_local(Expr&, const Token&);

    void begin_scope();
    void end_scope();

    void declare(const Token&);
    void define(const Token&);

    void visit(BlockStmt&) override;
    void visit(ClassStmt&) override;
    void visit(ExprStmt&) override;
    void visit(FnStmt&) override;
    void visit(IfStmt&) override;
    void visit(PrintStmt&) override;
    void visit(ReturnStmt&) override;
    void visit(VarStmt&) override;
    void visit(WhileStmt&) override;

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

public:
    Resolver(Interpreter& interpreter) : interpreter(interpreter) {}

    void resolve(const std::vector<std::unique_ptr<Stmt>>&);
};

};

#endif
