#include "parser.hpp"

#include "error.hpp"
#include "lox.hpp"

std::unique_ptr<lox::Expr> lox::Parser::expression() {
    return assignment();
}

std::unique_ptr<lox::Expr> lox::Parser::assignment() {
    std::unique_ptr<Expr> expr = logical_or();
    if (match({EQUAL})) {
        const Token&               equal = previous();
        std::unique_ptr<lox::Expr> value = assignment();
        if (auto* var = dynamic_cast<VariableExpr*>(expr.get()); var) {
            const Token& name = var->name;
            return std::make_unique<AssignExpr>(name, std::move(value));
        }
        if (auto* get = dynamic_cast<GetExpr*>(expr.get()); get)
            return std::make_unique<SetExpr>(std::move(get->object), std::move(value), get->name);
        throw error(equal, "Invalid Assignment Target");
    }
    return expr;
}

std::unique_ptr<lox::Expr> lox::Parser::logical_or() {
    std::unique_ptr<Expr> expr = logical_and();
    if (match({OR})) {
        const Token&          op    = previous();
        std::unique_ptr<Expr> right = logical_and();
        expr                        = std::make_unique<LogicalExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<lox::Expr> lox::Parser::logical_and() {
    std::unique_ptr<Expr> expr = equality();
    if (match({AND})) {
        const Token&          op    = previous();
        std::unique_ptr<Expr> right = equality();
        expr                        = std::make_unique<LogicalExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<lox::Expr> lox::Parser::equality() {
    std::unique_ptr<Expr> expr = comparison();
    while (match({EQUAL_EQUAL, BANG_EQUAL})) {
        const Token&          op    = previous();
        std::unique_ptr<Expr> right = comparison();
        expr                        = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<lox::Expr> lox::Parser::comparison() {
    std::unique_ptr<Expr> expr = term();
    while (match({GREATER, GREATER_EQUAL, LESSER, LESSER_EQUAL})) {
        const Token&          op    = previous();
        std::unique_ptr<Expr> right = term();
        expr                        = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<lox::Expr> lox::Parser::term() {
    std::unique_ptr<Expr> expr = factor();
    while (match({MINUS, PLUS})) {
        const Token&          op    = previous();
        std::unique_ptr<Expr> right = factor();
        expr                        = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<lox::Expr> lox::Parser::factor() {
    std::unique_ptr<Expr> expr = unary();
    while (match({STAR, SLASH})) {
        const Token&          op    = previous();
        std::unique_ptr<Expr> right = unary();
        expr                        = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<lox::Expr> lox::Parser::unary() {
    while (match({BANG, MINUS})) {
        const Token&          op    = previous();
        std::unique_ptr<Expr> right = call();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }
    return call();
}

std::unique_ptr<lox::Expr> lox::Parser::call() {
    std::unique_ptr<lox::Expr> expr = primary();
    while (true) {
        if (match({LEFT_PAREN})) {
            expr = finish_call(std::move(expr));
        } else if (match({DOT})) {
            const Token& name = consume(IDENTIFIER, "Expected property name after '.'.");
            expr              = std::make_unique<GetExpr>(std::move(expr), name);
        } else {
            break;
        }
    }
    return expr;
}

std::unique_ptr<lox::Expr> lox::Parser::finish_call(std::unique_ptr<lox::Expr> callee) {
    std::vector<std::unique_ptr<Expr>> arguments = std::vector<std::unique_ptr<Expr>>();
    if (!check(RIGHT_PAREN))
        do {
            if (arguments.size() == 255) // to make sure it's printed only once regardless of the number of arguments > 255
                error(peek(), "Can't have more than 255 arguments");
            arguments.emplace_back(expression());
        } while (match({COMMA}));
    const Token& paren = consume(RIGHT_PAREN, "Expected '(' after arguments");
    return std::make_unique<CallExpr>(std::move(callee), paren, std::move(arguments));
}

std::unique_ptr<lox::Expr> lox::Parser::primary() {
    if (match({FALSE}))
        return std::make_unique<LiteralExpr>(false);
    if (match({TRUE}))
        return std::make_unique<LiteralExpr>(true);
    if (match({NIL}))
        return std::make_unique<LiteralExpr>(Value{});
    if (match({NUMBER, STRING}))
        return std::make_unique<LiteralExpr>(previous().literal);
    if (match({IDENTIFIER}))
        return std::make_unique<VariableExpr>(previous());
    if (match({LEFT_PAREN})) {
        std::unique_ptr<lox::Expr> expr = expression();
        consume(RIGHT_PAREN, "Expected ')' after expression ");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }
    if (match({THIS}))
        return std::make_unique<ThisExpr>(previous());
    if (match({SUPER})) {
        const Token& keyword = previous();
        consume(DOT, "Expected '.' after super");
        const Token& method = consume(IDENTIFIER, "Expected superclass method name");
        return std::make_unique<SuperExpr>(keyword, method);
    }
    throw error(peek(), "Expected an expression");
}

std::unique_ptr<lox::Stmt> lox::Parser::class_declaration() {

    const Token&                  name = consume(IDENTIFIER, "Expected class name");
    std::unique_ptr<VariableExpr> superclass;

    if (match({LESSER})) {
        consume(IDENTIFIER, "Expected superclass name");
        superclass = std::make_unique<VariableExpr>(previous());
    }

    consume(LEFT_CURLY, "Expected '{' before class body");

    std::vector<std::unique_ptr<FnStmt>> methods;

    while (!check(RIGHT_CURLY) and !end())
        methods.emplace_back(function("method"));

    consume(RIGHT_CURLY, "Expected '}' after class body");

    return std::make_unique<ClassStmt>(name, std::move(methods), std::move(superclass));
}

std::unique_ptr<lox::Stmt> lox::Parser::declaration() {
    try {
        if (match({CLASS}))
            return class_declaration();
        if (match({FUN}))
            return function("function");
        if (match({VAR}))
            return var_declaration();
        return statement();
    } catch (const ParseError&) {
        synchronize();
        return {};
    }
}

std::unique_ptr<lox::FnStmt> lox::Parser::function(const std::string& kind) {
    const Token& name = consume(IDENTIFIER, "Expected " + kind + " name");
    consume(LEFT_PAREN, "Expected '(' after " + kind + " name");
    std::vector<Token> params;
    if (!check(RIGHT_PAREN)) {
        do {
            if (params.size() == 255)
                error(peek(), "Can't have more than 255 parameters");
            params.emplace_back(consume(IDENTIFIER, "Expected parameter name"));
        } while (match({COMMA}));
    }
    consume(RIGHT_PAREN, "Expected ')' after parameters");
    consume(LEFT_CURLY, "Expected '{' before " + kind + " body");
    std::vector<std::unique_ptr<Stmt>> body = block();
    return std::make_unique<FnStmt>(name, std::move(params), std::move(body));
}

std::unique_ptr<lox::Stmt> lox::Parser::var_declaration() {
    const Token&          name        = consume(IDENTIFIER, "Expected variable name");
    std::unique_ptr<Expr> initializer = match({EQUAL}) ? expression() : nullptr;
    consume(SEMICOLON, "Expected ';' after variable declaration");
    return std::make_unique<VarStmt>(name, std::move(initializer));
}

std::unique_ptr<lox::Stmt> lox::Parser::statement() {
    if (match({PRINT}))
        return print_statement();
    if (match({LEFT_CURLY}))
        return std::make_unique<BlockStmt>(block());
    if (match({IF}))
        return if_statement();
    if (match({WHILE}))
        return while_statement();
    if (match({FOR}))
        return for_statement();
    if (match({RETURN}))
        return return_statement();
    return expression_statement();
}

std::vector<std::unique_ptr<lox::Stmt>> lox::Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(RIGHT_CURLY) and !end())
        statements.emplace_back(declaration());
    consume(RIGHT_CURLY, "Expected '}' after block");
    return std::move(statements);
}

std::unique_ptr<lox::Stmt> lox::Parser::for_statement() {
    consume(LEFT_PAREN, "Expected '(' after for");
    std::unique_ptr<Stmt> init;
    if (match({SEMICOLON}))
        ;
    else if (match({VAR}))
        init = var_declaration();
    else
        init = expression_statement();
    std::unique_ptr<Expr> condition;
    if (!check({SEMICOLON}))
        condition = expression();
    consume(SEMICOLON, "Expected ';' after condition");
    std::unique_ptr<Expr> update;
    if (!check({RIGHT_PAREN}))
        update = expression();
    consume(RIGHT_PAREN, "Expected ')' after for clauses");
    std::unique_ptr<Stmt> body = statement();
    if (update) {
        std::vector<std::unique_ptr<Stmt>> loop_stmts;
        loop_stmts.emplace_back(std::move(body));
        loop_stmts.emplace_back(std::make_unique<ExprStmt>(std::move(update)));
        body = std::make_unique<BlockStmt>(std::move(loop_stmts));
    }
    body = std::make_unique<WhileStmt>(std::move(condition), std::move(body));
    if (init) {
        std::vector<std::unique_ptr<Stmt>> block;
        block.emplace_back(std::move(init));
        block.emplace_back(std::move(body));
        body = std::make_unique<BlockStmt>(std::move(block));
    }
    return std::move(body);
}

std::unique_ptr<lox::Stmt> lox::Parser::if_statement() {
    consume(LEFT_PAREN, "Expected '(' after if");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expected ')' after condition");
    std::unique_ptr<Stmt> then = statement();
    std::unique_ptr<Stmt> otherwise;
    if (match({ELSE}))
        otherwise = statement();
    return std::make_unique<IfStmt>(std::move(condition), std::move(then), std::move(otherwise));
}

std::unique_ptr<lox::Stmt> lox::Parser::print_statement() {
    std::unique_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expected ';' after value");
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<lox::Stmt> lox::Parser::expression_statement() {
    std::unique_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expected ';' after value");
    return std::make_unique<ExprStmt>(std::move(value));
}

std::unique_ptr<lox::Stmt> lox::Parser::return_statement() {
    const Token&          keyword = previous();
    std::unique_ptr<Expr> expr;
    if (!check(SEMICOLON))
        expr = expression();
    consume(SEMICOLON, "Expected ';' after value");
    return std::make_unique<ReturnStmt>(keyword, std::move(expr));
}

std::unique_ptr<lox::Stmt> lox::Parser::while_statement() {
    consume(LEFT_PAREN, "Expected '(' after while");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expected ')' after condition");
    std::unique_ptr<Stmt> body = statement();
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

template <class token_type> bool lox::Parser::match(std::initializer_list<token_type> types) {
    for (const TokenType type : types)
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

lox::Token& lox::Parser::advance() {
    if (!end())
        current++;
    return previous();
}

lox::Token& lox::Parser::peek() {
    return tokens[current];
}

lox::Token& lox::Parser::previous() {
    return tokens[current - 1];
}

lox::Token& lox::Parser::consume(const TokenType type, const std::string& message) {
    if (check(type))
        return advance();
    throw error(peek(), message);
}

lox::Parser::ParseError lox::Parser::error(const lox::Token& token, const std::string& message) {
    lox::error(token, message);
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

std::vector<std::unique_ptr<lox::Stmt>> lox::Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!end())
        try {
            statements.emplace_back(declaration());
        } catch (const ParseError&) {
            return statements;
        }
    return statements;
}
