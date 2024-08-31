#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "token.hpp"

#include <string>
#include <vector>

namespace lox {

class Scanner {

    const std::string& source;
    const size_t       length;
    std::vector<Token> tokens;

    const char* src_start;
    const char* src_end;
    const char* start;
    const char* current;

    size_t line = 1;

    char advance();
    bool match(const char);
    char peek() const;
    char peek_next() const;

    void add_token(const TokenType);
    void add_token(const TokenType, Value);

    bool is_digit(const char) const;
    bool is_alpha_or_underscore(const char) const;
    bool is_alnum_or_underscore(const char) const;

    void check_string();
    void check_number();
    void check_identifier();

    void scan_token();

public:
    Scanner(const std::string& source)
        : source(source), length(source.length()), src_start(&source[0]), start(src_start), src_end(src_start + length), current(start) {}
    std::vector<Token> scan_tokens();
};

};

#endif
