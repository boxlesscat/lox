#include "lox.hpp"

#include "error.hpp"
#include "interpreter.hpp"
#include "parser.hpp"
#include "resolver.hpp"
#include "scanner.hpp"

#include <fstream>
#include <iostream>
#include <memory>

lox::Interpreter interpreter;

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "usage lox [script]";
        return 64;
    }
    if (argc == 2)
        lox::run_file(argv[1]);
    else
        lox::run_prompt();
    return 0;
}

void lox::run_file(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        std::cerr << "No such file or directory\n";
        exit(66);
    }
    const std::string source = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    run(source);
    if (had_error)
        exit(65);
    if (had_runtime_error)
        exit(70);
}

void lox::run_prompt() {
    std::string source;
    for (;;) {
        had_error = false;
        std::cout << ">> ";
        std::cout.flush();
        std::getline(std::cin, source);
        run(source);
    }
}

void lox::run(const std::string& source) {
    Scanner            scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();
    if (had_error)
        return;
    Parser                             parser(tokens);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();
    if (had_error)
        return;
    Resolver resolver(interpreter);
    resolver.resolve(statements);
    if (had_error)
        return;
    interpreter.interpret(statements);
}

void lox::report(const int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
}
