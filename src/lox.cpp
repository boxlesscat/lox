#include <iostream>
#include <fstream>
#include "lox.hpp"
#include "scanner.hpp"


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
    for (Token token : tokens)
        std::cout << token << "\n";
}

void lox::error(const std::string message, const int line) {
    hadError = true;
    report(line, "", message);
}

void lox::report(const int line, std::string where, const std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
}
