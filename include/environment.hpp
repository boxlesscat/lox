#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <memory>
#include <unordered_map>
#include "token.hpp"


namespace lox {

class Environment {



public:
    std::unordered_map<std::string, std::any> values;
    const std::shared_ptr<Environment> enclosing;

    Environment(const std::shared_ptr<Environment> enclosing = nullptr) : enclosing(enclosing) {}

    void define(const Token, const std::any);
    void assign(const Token, const std::any);

    std::any get(const Token);

};

};

#endif
