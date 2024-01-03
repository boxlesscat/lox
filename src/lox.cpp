#include <iostream>
#include <fstream>
#include "lox.hpp"


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
}

void lox::run_prompt() {
    std::string source;
    for (;;) {
        std::cout << ">> ";
        std::getline(std::cin, source);
        lox::run(source);
    }
}

void lox::run(const std::string& source) {
    // ..
}
