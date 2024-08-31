#ifndef PARSER_HPP
#define PARSER_HPP

#include "expression.hpp"
#include "stmt.hpp"
#include "token.hpp"

#include <vector>

namespace lox {

class Parser {

    std::vector<Token> tokens;
    int                current = 0;

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> logical_or();
    std::unique_ptr<Expr> logical_and();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> call();
    std::unique_ptr<Expr> finish_call(std::unique_ptr<lox::Expr> callee);
    std::unique_ptr<Expr> primary();

    std::unique_ptr<Stmt>   class_declaration();
    std::unique_ptr<Stmt>   declaration();
    std::unique_ptr<FnStmt> function(const std::string& kind);
    std::unique_ptr<Stmt>   var_declaration();
    std::unique_ptr<Stmt>   statement();
    std::unique_ptr<Stmt>   for_statement();
    std::unique_ptr<Stmt>   if_statement();
    std::unique_ptr<Stmt>   print_statement();
    std::unique_ptr<Stmt>   while_statement();
    std::unique_ptr<Stmt>   return_statement();
    std::unique_ptr<Stmt>   expression_statement();

    std::vector<std::unique_ptr<Stmt>> block();

    template <class token_type> bool match(std::initializer_list<token_type>);
    bool                             check(const TokenType);
    bool                             end();

    Token& advance();
    Token& peek();
    Token& previous();
    Token& consume(const TokenType type, const std::string& message);

    struct ParseError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    ParseError error(const Token& token, const std::string& message);

    void synchronize();

public:
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}
    std::vector<std::unique_ptr<Stmt>> parse();
};

};

#endif
