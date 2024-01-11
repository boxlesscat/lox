#include "error.hpp"
#include "lox.hpp"


bool lox::hadError = false;
bool lox::hadRuntimeError = false;

void lox::error(const std::string message, const int line) {
    hadError = true;
    report(line, "", message);
}

void lox::error(const lox::Token token, std::string&& message) {
    hadError = true;
    if (token.type == END)
        report(token.line, " at end", std::move(message));
    else
        report(token.line, " at '" + token.lexeme + "'", message);
}

void lox::runtime_error(const lox::RuntimeError error) {
    std::cerr << error.what() << " [line " << error.token.line << "]\n";
    hadRuntimeError = true;
}
