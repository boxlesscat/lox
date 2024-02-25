#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <memory>
#include <unordered_map>
#include "token.hpp"


namespace lox {

struct Environment : public std::enable_shared_from_this<Environment> {

    std::unordered_map<std::string, std::any> values;
    const std::shared_ptr<Environment> enclosing;
    
public:

    Environment(const std::shared_ptr<Environment> enclosing = nullptr) : enclosing(enclosing) {}

    void define(const std::string, const std::any);
    void assign(const Token, const std::any);
    void assign_at(const int, Token, std::any);

    std::shared_ptr<Environment> ancestor(const int);
    std::any get(const Token);
    std::any get_at(const int, std::string);

};

};

#endif
