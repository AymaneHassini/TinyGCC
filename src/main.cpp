#include <iostream>
#include <fstream>
#include <sstream>

#include "tokenization.hpp"
#include "parser.hpp"
#include "generation.hpp"

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cerr << "Usage: tiny <file>\n";
        return EXIT_FAILURE;
    }

    std::ifstream input(argv[1]);
    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string contents = buffer.str();

    Tokenizer tokenizer(contents);
    auto tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    auto tree = parser.parse();

    if (!tree) {
        std::cerr << "No return statement found\n";
        return EXIT_FAILURE;
    }

    Generator generator(tree.value());

    std::ofstream file("out.asm");
    file << generator.generate();

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}
