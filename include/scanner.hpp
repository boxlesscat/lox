#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <vector>
#include "token.hpp"


namespace lox {

class Scanner {
    
    const std::string source;
    const unsigned length;
    std::vector<Token> tokens;
    
    unsigned start = 0;
    unsigned current = 0;
    unsigned line = 1; 
    
    char advance();
    bool match(const char);
    char peek() const;
    char peek_next() const;

    void add_token(const TokenType);
    void add_token(const TokenType, const std::any);

    bool is_digit(const char) const;
    bool is_alpha_(const char) const;
    bool is_alnum_(const char) const;

    void check_string();
    void check_number();
    void check_identifier();

    void scan_token();

public:
    Scanner(const std::string source) : source(source), length(source.length()) {}
    std::vector<Token> scan_tokens();
};

};

#endif
