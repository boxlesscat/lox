#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace lox {

enum TokenType {
    // one char tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_CURLY,
    RIGHT_CURLY,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    STAR,
    SLASH,
    // one or two char tokens
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESSER,
    LESSER_EQUAL,
    // literals
    STRING,
    NUMBER,
    IDENTIFIER,
    // keywords
    AND,
    OR,
    IF,
    ELSE,
    FOR,
    WHILE,
    NIL,
    TRUE,
    FALSE,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    VAR,
    CLASS,
    FUN,
    // other
    END,
};

const std::string tokentypes[] = {
    "(",   ")",     "{",   "}",    ",",     ".",     "-",      "+",      ";",          "*",   "/",     "!",   "!=",
    "=",   "==",    ">",   ">=",   "<",     "<=",    "string", "number", "identifier", "and", "or",    "if",  "else",
    "for", "while", "nil", "true", "false", "print", "return", "super",  "this",       "var", "class", "fun",
};

static std::unordered_map<std::string, TokenType> keywords = {
    {   "and",    AND},
    {    "or",     OR},
    {    "if",     IF},
    {  "else",   ELSE},
    {   "for",    FOR},
    { "while",  WHILE},
    {   "nil",    NIL},
    {  "true",   TRUE},
    { "false",  FALSE},
    { "print",  PRINT},
    {"return", RETURN},
    { "super",  SUPER},
    {  "this",   THIS},
    {   "var",    VAR},
    { "class",  CLASS},
    {   "fun",    FUN},
};

struct LoxCallable;
struct LoxInstance;

using Value = std::variant<std::monostate, bool, double, std::string, std::shared_ptr<LoxCallable>, std::shared_ptr<LoxInstance>>;

struct Token {
    const TokenType   type;
    const std::string lexeme;
    const Value       literal;
    const unsigned    line;
    Token(const TokenType type, std::string lexeme, Value literal, const unsigned line)
        : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}
    std::string          to_string() const;
    friend std::ostream& operator<<(std::ostream&, const Token&);
};

std::ostream& operator<<(std::ostream&, const lox::Token&);

};

#endif
