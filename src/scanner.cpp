#include "scanner.hpp"
#include "token.hpp"
#include "lox.hpp"


char lox::Scanner::advance() {
    return source[current++];
}

bool lox::Scanner::match(const char expected) {
    if (current == length or source[current] != expected)
        return false;
    current++;
    return true; 
}

char lox::Scanner::peek() const {
    if (current == length)
        return 0;
    return source[current];
}

char lox::Scanner::peek_next() const {
    if (current + 1 >= length)
        return 0;
    return source[current + 1];
}

void lox::Scanner::add_token(const lox::TokenType type) {
    add_token(type, nullptr);
}

void lox::Scanner::add_token(const lox::TokenType type, const std::any literal) {
    tokens.emplace_back(type, source.substr(start, current - start), literal, line);
}

bool lox::Scanner::is_digit(const char ch) const {
    return '0' <= ch and ch <= '9';
}

bool lox::Scanner::is_alpha_(const char ch) const {
    return ch == '_' or 'a' <= ch and ch <= 'z' or 'A' <= ch and ch <= 'Z';
}

bool lox::Scanner::is_alnum_(const char ch) const {
    return is_digit(ch) or is_alpha_(ch);
}

void lox::Scanner::check_string() {
    while (peek() != '"' and current != length)
        advance();
    if (current == length) {
        error("Unterminated string", line);
        return;
    }
    advance();
    add_token(STRING, source.substr(start + 1, current - start - 2));
}

void lox::Scanner::check_number() {
    while (is_digit(peek()))
        advance();
    if (peek() == '.' and is_digit(peek_next())) {
        advance();
        while (is_digit(peek()))
            advance();
    }
    add_token(NUMBER, std::stod(source.substr(start, current - start)));
}

void lox::Scanner::check_identifier() {
    while (is_alnum_(peek()))
        advance();
    const std::string value = source.substr(start, current - start);
    if (keywords.find(value) == keywords.end())
        add_token(IDENTIFIER, value);
    else
        add_token(lox::keywords[value]);
}

void lox::Scanner::scan_token() {
    char ch = advance();
    switch (ch) {
        case '(':
            add_token(LEFT_PAREN);
            break;
        case ')':
            add_token(RIGHT_PAREN);
            break;
        case '{':
            add_token(LEFT_CURLY);
            break;
        case '}':
            add_token(RIGHT_CURLY);
            break;
        case ',':
            add_token(COMMA);
            break;
        case '.':
            add_token(DOT);
            break;
        case '-':
            add_token(MINUS);
            break;
        case '+':
            add_token(PLUS);
            break;
        case ';':
            add_token(SEMICOLON);
            break;
        case '*':
            add_token(STAR);
            break;
        case '!':
            add_token(match('=') ? BANG_EQUAL : BANG);
            break;
        case '=':
            add_token(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '>':
            add_token(match('=') ? GREATER_EQUAL : GREATER);
            break;
        case '<':
            add_token(match('=') ? LESSER_EQUAL : LESSER);
            break;
        case '/':
            if (peek() == '/')
                while (peek() != '\n' and current != length)
                    advance();
            else 
                add_token(SLASH);
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '"':
            check_string();
            break;
        default:
            if (is_digit(ch))
                check_number();
            else if (is_alpha_(ch))
                check_identifier();
            else
                error("Unexpected character", line);
    }
}

std::vector<lox::Token> lox::Scanner::scan_tokens() {
    while (current < length) {
        start = current;
        scan_token();
    }
    tokens.emplace_back(END, "", nullptr, line);
    return tokens;
}
