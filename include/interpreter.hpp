#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "expression.hpp"


namespace lox {

class Interpreter : Visitor {

    bool is_truthy(const std::any) const;
    bool is_equal(const std::any, const std::any) const;

    void check_number_operand(const Token, const std::any) const;
    void check_number_operands(const Token, const std::any, const std::any) const;

    std::any evaluate(const std::shared_ptr<Expr>);

public:
    std::any visitBinaryExpr(const std::shared_ptr<BinaryExpr>) override;
    std::any visitGroupingExpr(const std::shared_ptr<GroupingExpr>) override;
    std::any visitLiteralExpr(const std::shared_ptr<LiteralExpr>) override;
    std::any visitUnaryExpr(const std::shared_ptr<UnaryExpr>) override;

    void interpret(const std::shared_ptr<Expr>);
    std::string stringfy(const std::any) const;

};

};

#endif
