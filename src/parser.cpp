#include "parser.hpp"
#include "lox.hpp"
#include "error.hpp"


std::shared_ptr<lox::Expr> lox::Parser::expression() {
    return equality();
}

std::shared_ptr<lox::Expr> lox::Parser::equality() {
    std::shared_ptr<Expr> expr = comparision();
    while (match({EQUAL_EQUAL, BANG_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> right = comparision();
        expr = std::make_shared<BinaryExpr>(BinaryExpr(expr, op, right));
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::comparision() {
    std::shared_ptr<Expr> expr = term();
    while (match({GREATER, GREATER_EQUAL, LESSER, LESSER_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<BinaryExpr>(BinaryExpr(expr, op, right));
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::term() {
    std::shared_ptr<Expr> expr = factor();
    while (match({MINUS, PLUS})) {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<BinaryExpr>(BinaryExpr(expr, op, right));
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::factor() {
    std::shared_ptr<Expr> expr = unary();
    while (match({STAR, SLASH})) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<BinaryExpr>(BinaryExpr(expr, op, right));
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::unary() {
    while (match({BANG, MINUS})) {
        Token op = previous();
        std::shared_ptr<Expr> right = primary();
        return std::make_shared<UnaryExpr>(UnaryExpr(op, right));
    }
    return primary();
}

std::shared_ptr<lox::Expr> lox::Parser::primary() {
    if (match({FALSE}))
        return std::make_shared<LiteralExpr>(LiteralExpr(false));
    if (match({TRUE}))
        return std::make_shared<LiteralExpr>(LiteralExpr(true));
    if (match({NIL}))
        return std::make_shared<LiteralExpr>(LiteralExpr(nullptr));
    if (match({NUMBER, STRING}))
        return std::make_shared<LiteralExpr>(LiteralExpr(previous().literal));
    if (match({LEFT_PAREN})) {
        std::shared_ptr<lox::Expr> expr = expression();
        consume(RIGHT_PAREN, "Expected ')' after expression ");
        return std::make_shared<GroupingExpr>(GroupingExpr(expr));
    }
    throw error(peek(), "Expected an expression");
}

template<class token_type>
bool lox::Parser::match(std::initializer_list<token_type> types) {
    for (TokenType type : types)
        if (check(type)) {
            advance();
            return true;
        }
    return false;
}

bool lox::Parser::check(const lox::TokenType type) {
    return !end() and peek().type == type;
}

bool lox::Parser::end() {
    return peek().type == END;
}

lox::Token lox::Parser::advance() {
    if (!end())
        current++;
    return previous();
}

lox::Token lox::Parser::peek() {
    return tokens[current];
}

lox::Token lox::Parser::previous() {
    return tokens[current - 1];
}

lox::Token lox::Parser::consume(const TokenType type, std::string&& message) {
    if (check(type))
        return advance();
    throw error(peek(), std::move(message));
}

lox::Parser::ParseError lox::Parser::error(const lox::Token token, std::string&& message) {
    lox::error(token, std::move(message));
    return ParseError{""};
}

void lox::Parser::synchronize() {
    advance();
    while (!end()) {
        if (previous().type == SEMICOLON)
            return;
        switch (peek().type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
        }
        advance();
    }
}

std::shared_ptr<lox::Expr> lox::Parser::parse() {
    try {
        return expression();
    } catch (ParseError error) {
        return nullptr;
    }
}