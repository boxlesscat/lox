#include "token.hpp"


std::string lox::Token::to_string() const {
    if (type == NUMBER)
        return lox::tokentypes[type] + " " + lexeme + " (" + std::to_string(std::any_cast<double>(literal)) + ")";
    return lox::tokentypes[type] + " " + lexeme;
}

std::ostream& lox::operator<<(std::ostream& ost, const lox::Token& token) {
    return ost << token.to_string();
}

std::ostream& lox::operator<<(std::ostream& ost, const lox::Token&& token) {
    return ost << token.to_string();
}
