#include "token.hpp"


std::string lox::Token::to_string() const {
    switch (type) {
        case NUMBER:
            return lox::tokentypes[type] + " " + lexeme + " (" + std::to_string(std::any_cast<double>(literal)) + ")" + " " + std::to_string(line);
        case STRING:
            return lox::tokentypes[type] + " " + lexeme + " (" + std::any_cast<std::string>(literal) + ")" + " " + std::to_string(line);
        case IDENTIFIER:
            return lox::tokentypes[type] + " " + lexeme + " " + std::to_string(line);
        default:
            return lox::tokentypes[type] + " " + std::to_string(line);
    }
}

std::ostream& lox::operator<<(std::ostream& ost, const lox::Token& token) {
    return ost << token.to_string();
}

std::ostream& lox::operator<<(std::ostream& ost, const lox::Token&& token) {
    return ost << token.to_string();
}
