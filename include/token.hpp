#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>
#include <any>
#include <unordered_map>


namespace lox {

enum TokenType {
    // one char tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_CURLY, RIGHT_CURLY,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, STAR, SLASH,
    // one or two char tokens
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESSER, LESSER_EQUAL,
    // literals
    STRING, NUMBER, IDENTIFIER,
    // keywords
    AND, OR, IF, ELSE, FOR, WHILE, NIL, TRUE, FALSE,
    PRINT, RETURN, SUPER, THIS, VAR, CLASS, FUN,
};

const std::string tokentypes[] = {
    "(", ")", "{", "}",
    ",", ".", "-", "+", ";", "*", "/",
    "!", "!=", "=", "==",
    ">", ">=", "<", "<=",
    "string", "number", "indentifier",
    "and", "or", "if", "else", "for", "while", "nil", "true", "false",
    "print", "return", "super", "this", "var", "class", "fun",
};

static std::unordered_map<std::string, TokenType> keywords = {
    {("and"), AND},
    {"or", OR},
    {"if", IF},
    {"else", ELSE},
    {"for", FOR},
    {"while", WHILE},
    {"nil", NIL},
    {"true", TRUE},
    {"false", FALSE},
    {"print", PRINT},
    {"return",RETURN},
    {"super", SUPER},
    {"this", THIS},
    {"var", VAR},
    {"class", CLASS},
    {"fun", FUN},
};

struct Token {
    const TokenType type;
    const std::string lexeme;
    const std::any literal;
    const unsigned line;
    Token(const TokenType type, const std::string lexeme, const std::any literal, const unsigned line) :
        type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {
    }
    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream&, const Token&);
    friend std::ostream& operator<<(std::ostream&, const Token&&);
};

std::ostream& operator<<(std::ostream&, const lox::Token&);
std::ostream& operator<<(std::ostream&, const lox::Token&&);

};


#endif
