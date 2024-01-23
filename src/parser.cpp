#include "parser.hpp"
#include "lox.hpp"
#include "error.hpp"


std::shared_ptr<lox::Expr> lox::Parser::expression() {
    return assignment();
}

std::shared_ptr<lox::Expr> lox::Parser::assignment() {
    std::shared_ptr<Expr> expr = logical_or();
    if (match({EQUAL})) {
        Token equal = previous();
        std::shared_ptr<lox::Expr> value = assignment();
        if (typeid(*expr) == typeid(VariableExpr)) {
            Token name = std::dynamic_pointer_cast<VariableExpr>(expr) -> name;
            return std::make_shared<AssignExpr>(name, value);
        }
        throw error(equal, "Invalid Assignment Target");
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::logical_or() {
    std::shared_ptr<Expr> expr = logical_and();
    if (match({OR})) {
        Token op = previous();
        std::shared_ptr<Expr> right = logical_and();
        expr = std::make_shared<LogicalExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::logical_and() {
    std::shared_ptr<Expr> expr = equality();
    if (match({AND})) {
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<LogicalExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::equality() {
    std::shared_ptr<Expr> expr = comparision();
    while (match({EQUAL_EQUAL, BANG_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> right = comparision();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::comparision() {
    std::shared_ptr<Expr> expr = term();
    while (match({GREATER, GREATER_EQUAL, LESSER, LESSER_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::term() {
    std::shared_ptr<Expr> expr = factor();
    while (match({MINUS, PLUS})) {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::factor() {
    std::shared_ptr<Expr> expr = unary();
    while (match({STAR, SLASH})) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::unary() {
    while (match({BANG, MINUS})) {
        Token op = previous();
        std::shared_ptr<Expr> right = call();
        return std::make_shared<UnaryExpr>(op, right);
    }
    return call();
}

std::shared_ptr<lox::Expr> lox::Parser::call() {
    std::shared_ptr<lox::Expr> expr = primary();
    while (match({LEFT_PAREN}))
        expr = finish_call(expr);
    return expr;
}

std::shared_ptr<lox::Expr> lox::Parser::finish_call(const std::shared_ptr<lox::Expr> callee) {
    std::shared_ptr<std::vector<std::shared_ptr<Expr>>> arguments = std::make_shared<std::vector<std::shared_ptr<Expr>>>();
    if (!check(RIGHT_PAREN))
        do {
            if (arguments -> size() == 255) // to make sure it's printed only once regardless of the number of arguments > 255
                error(peek(), "Can't have more than 255 arguments");
            arguments -> emplace_back(expression());
        }  while (match({COMMA}));
    Token paren = consume(RIGHT_PAREN, "Expected '(' after arguments");
    return std::make_shared<CallExpr>(callee, paren, arguments);
}

std::shared_ptr<lox::Expr> lox::Parser::primary() {
    if (match({FALSE}))
        return std::make_shared<LiteralExpr>(false);
    if (match({TRUE}))
        return std::make_shared<LiteralExpr>(true);
    if (match({NIL}))
        return std::make_shared<LiteralExpr>(nullptr);
    if (match({NUMBER, STRING}))
        return std::make_shared<LiteralExpr>(previous().literal);
    if (match({IDENTIFIER}))
        return std::make_shared<VariableExpr>(previous());
    if (match({LEFT_PAREN})) {
        std::shared_ptr<lox::Expr> expr = expression();
        consume(RIGHT_PAREN, "Expected ')' after expression ");
        return std::make_shared<GroupingExpr>(expr);
    }
    throw error(peek(), "Expected an expression");
}

std::shared_ptr<lox::Stmt> lox::Parser::declaration() {
    try {
        if (match({VAR}))
            return var_declaration();
        return statement();
    } catch(ParseError) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<lox::Stmt> lox::Parser::var_declaration() {
    Token name = consume(IDENTIFIER, "Expected variable name");
    std::shared_ptr<Expr> initializer = match({EQUAL}) ? expression() : nullptr;
    consume(SEMICOLON, "Expected ';' after variable declaration");
    return std::make_shared<VarStmt>(name, initializer);
}

std::shared_ptr<lox::Stmt> lox::Parser::statement() {
    if (match({PRINT}))
        return print_statement();
    if (match({LEFT_CURLY}))
        return std::make_shared<BlockStmt>(block());
    if (match({IF}))
        return if_statement();
    if (match({WHILE}))
        return while_statement();
    if (match({FOR}))
        return for_statement();
    return expression_statement();
}

std::shared_ptr<std::vector<std::shared_ptr<lox::Stmt>>> lox::Parser::block() {
    using vec = std::vector<std::shared_ptr<Stmt>>;
    std::shared_ptr<vec> statements = std::make_shared<vec>();
    while (!check(RIGHT_CURLY) and !end())
        statements -> emplace_back(declaration());
    consume(RIGHT_CURLY, "Expected '{' after block");
    return statements;
}

std::shared_ptr<lox::Stmt> lox::Parser::for_statement() {
    consume(LEFT_PAREN, "Expected '(' after for");
    std::shared_ptr<Stmt> init = nullptr;
    if (match({SEMICOLON}))
        ;
    else if (match({VAR}))
        init = var_declaration();
    else
        init = expression_statement();
    std::shared_ptr<Expr> condition = nullptr;
    if (!check({SEMICOLON}))
        condition = expression();
    consume(SEMICOLON, "Expected ';' after condition");
    std::shared_ptr<Expr> update = nullptr;
    if (!check({RIGHT_PAREN}))
        update = expression();
    consume(RIGHT_PAREN, "Expected ')' after for clauses");
    std::shared_ptr<Stmt> body = statement();
    if (update != nullptr) {
        std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> vec = std::make_shared<std::vector<std::shared_ptr<Stmt>>>();
        if (typeid(*body) == typeid(BlockStmt))
            vec = std::dynamic_pointer_cast<BlockStmt>(body) -> statements;
        else
            vec -> emplace_back(body);
        vec -> emplace_back(std::make_shared<ExprStmt>(update));
        body = std::make_shared<BlockStmt>(vec);
    }
    body = std::make_shared<WhileStmt>(condition, body);
    if (init != nullptr) {
        std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> vec = std::make_shared<std::vector<std::shared_ptr<Stmt>>>();
        vec -> emplace_back(init);
        vec -> emplace_back(body);
        body = std::make_shared<BlockStmt>(vec);
    }
    return body;
}

std::shared_ptr<lox::Stmt> lox::Parser::if_statement() {
    consume(LEFT_PAREN, "Expected '(' after if");
    std::shared_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expected ')' after condition");
    std::shared_ptr<Stmt> then = statement();
    std::shared_ptr<Stmt> otherwise = nullptr;
    if (match({ELSE}))
        otherwise = statement();
    return std::make_shared<IfStmt>(condition, then, otherwise);
}

std::shared_ptr<lox::Stmt> lox::Parser::print_statement() {
    std::shared_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expected ';' after value");
    return std::make_shared<PrintStmt>(value);
}

std::shared_ptr<lox::Stmt> lox::Parser::expression_statement() {
    std::shared_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expected ';' after value");
    return std::make_shared<ExprStmt>(value);
}

std::shared_ptr<lox::Stmt> lox::Parser::while_statement() {
    consume(LEFT_PAREN, "Expected '(' after while");
    std::shared_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expected ')' after condition");
    std::shared_ptr<Stmt> body = statement();
    return std::make_shared<WhileStmt>(condition, body);
}

template<class token_type>
bool lox::Parser::match(std::initializer_list<token_type> types) {
    for (TokenType type : types)
        if (check(type)) {
            advance();
            return true;
        }
    return false;
}

bool lox::Parser::check(const lox::TokenType type) {
    return !end() and peek().type == type;
}

bool lox::Parser::end() {
    return peek().type == END;
}

lox::Token lox::Parser::advance() {
    if (!end())
        current++;
    return previous();
}

lox::Token lox::Parser::peek() {
    return tokens[current];
}

lox::Token lox::Parser::previous() {
    return tokens[current - 1];
}

lox::Token lox::Parser::consume(const TokenType type, std::string&& message) {
    if (check(type))
        return advance();
    throw error(peek(), std::move(message));
}

lox::Parser::ParseError lox::Parser::error(const lox::Token token, std::string&& message) {
    lox::error(token, std::move(message));
    return ParseError{""};
}

void lox::Parser::synchronize() {
    advance();
    while (!end()) {
        if (previous().type == SEMICOLON)
            return;
        switch (peek().type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
        }
        advance();
    }
}

std::shared_ptr<std::vector<std::shared_ptr<lox::Stmt>>> lox::Parser::parse() {
    using vec = std::vector<std::shared_ptr<Stmt>>;
    std::shared_ptr<vec> statements = std::make_shared<vec>();
    while (!end())
        try {
            statements -> emplace_back(declaration());
        } catch(ParseError) {
            return statements;
        }
    return statements;
}
