#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "token.hpp"

#include <memory>
#include <unordered_map>

namespace lox {

struct Environment : public std::enable_shared_from_this<Environment> {

    std::unordered_map<std::string, Value> values;
    std::shared_ptr<Environment>           enclosing;

public:
    Environment(std::shared_ptr<Environment> enclosing = nullptr) : enclosing(std::move(enclosing)) {}

    void define(const std::string&, Value);
    void assign(const Token&, Value);
    void assign_at(const int, const Token&, Value);

    std::shared_ptr<Environment> ancestor(const int);
    Value                        get(const Token&);
    Value                        get_at(const int, const std::string&);
};

};

#endif
