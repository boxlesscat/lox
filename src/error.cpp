#include "error.hpp"

#include "lox.hpp"

bool lox::had_error         = false;
bool lox::had_runtime_error = false;

void lox::error(const std::string& message, const int line) {
    had_error = true;
    report(line, "", message);
}

void lox::error(const lox::Token& token, const std::string& message) {
    had_error = true;
    if (token.type == END)
        report(token.line, " at end", message);
    else
        report(token.line, " at '" + token.lexeme + "'", message);
}

void lox::runtime_error(const lox::RuntimeError& error) {
    std::cerr << error.what() << " [line " << error.token.line << "]\n";
    had_runtime_error = true;
}
