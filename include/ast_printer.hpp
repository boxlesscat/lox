#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include "expression.hpp"
#include <sstream>


namespace lox {

class ASTPrinter : public ExprVisitor {
    template <class T> std::string parenthesize(const std::string name, std::initializer_list<T> exprs) {
        std::ostringstream oss;
        oss << "(" << name;
        for (auto expr : exprs)
            oss << " " << print(expr);
        oss << ")";
        return oss.str();
    }
public:
    std::string print(const std::shared_ptr<Expr> expr);
    std::any visit_binary_expr(const std::shared_ptr<BinaryExpr> expr) override;
    std::any visit_grouping_expr(const std::shared_ptr<GroupingExpr> expr) override;
    std::any visit_literal_expr(const std::shared_ptr<LiteralExpr> expr) override;
    std::any visit_unary_expr(const std::shared_ptr<UnaryExpr> expr) override;
};

};

#endif
