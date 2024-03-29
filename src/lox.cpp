#include <iostream>
#include <fstream>
#include "lox.hpp"
#include "scanner.hpp"
#include "error.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "resolver.hpp"


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
    run(std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()));
    if (hadError)
        exit(65);
    if (hadRuntimeError)
        exit(70);
}

void lox::run_prompt() {
    std::string source;
    for (;;) {
        hadError = false;
        std::cout << ">> ";
        std::getline(std::cin, source);
        run(source);
    }
}

void lox::run(const std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();
    if (hadError)
        return;
    Parser parser(tokens);
    std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> statements = parser.parse();
    if (hadError)
        return;
    Resolver resolver(interpreter);
    resolver.resolve(statements);
    if (hadError)
        return;
    interpreter.interpret(*statements);
}

void lox::report(const int line, std::string where, const std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
}
