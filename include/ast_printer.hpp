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
    std::any visitBinaryExpr(const std::shared_ptr<BinaryExpr> expr) override;
    std::any visitGroupingExpr(const std::shared_ptr<GroupingExpr> expr) override;
    std::any visitLiteralExpr(const std::shared_ptr<LiteralExpr> expr) override;
    std::any visitUnaryExpr(const std::shared_ptr<UnaryExpr> expr) override;
};

};

#endif
