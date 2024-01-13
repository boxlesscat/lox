#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "token.hpp"
#include "expression.hpp"
#include "stmt.hpp"


namespace lox {

class Parser {
    
    const std::vector<Token>& tokens;
    int current = 0;

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparision();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();

    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> print_statement();
    std::shared_ptr<Stmt> expression_statement();

    template<class token_type>
    bool match(std::initializer_list<token_type>);
    bool check(const TokenType);
    bool end();

    Token advance();
    Token peek();
    Token previous();
    Token consume(const TokenType, std::string&&);

    struct ParseError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    ParseError error(const Token, std::string&&);

    void synchronize();

public:
    Parser(std::vector<Token>& tokens) : tokens(tokens) {}
    std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> parse();
};

};

#endif