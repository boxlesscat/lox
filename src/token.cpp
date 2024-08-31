#include "token.hpp"

#include <iomanip>
#include <sstream>

std::string lox::Token::to_string() const {
    std::ostringstream oss;
    oss << std::setw(5);
    switch (type) {
    case END:
        oss << line << std::setw(15) << "end of file ";
        break;
    case NUMBER:
        oss << line << std::setw(15) << lox::tokentypes[type] << std::setw(10) << lexeme << std::setw(10) << " (" << std::get<double>(literal)
            << ") ";
        break;
    case STRING:
        oss << line << std::setw(15) << lox::tokentypes[type] << std::setw(10) << lexeme << std::setw(10) << " (" << std::get<std::string>(literal)
            << ") ";
        break;
    case IDENTIFIER:
        oss << line << std::setw(15) << lox::tokentypes[type] << std::setw(10) << lexeme;
        break;
    default:
        oss << line << std::setw(15) << lox::tokentypes[type];
    }
    return oss.str();
}

std::ostream& lox::operator<<(std::ostream& ost, const lox::Token& token) {
    return ost << token.to_string();
}
