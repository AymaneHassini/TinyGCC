#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include "./parser.hpp"
#include "./tokenization.hpp"
#include "./generation.hpp"


std::vector<Token> tokenize(const std::string& str)
{
    std::vector<Token> tokens;

    std::string buf;
    for (int i = 0; i < str.length(); i++)
    {
        char c = str.at(i);
        if (std::isalpha(c))
        {
            buf.push_back(c);
            i++;
            while (std::isalnum(str.at(i)))
            {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            if (buf == "return")
            {
                tokens.push_back({.type = TokenType::_return});
                buf.clear();
                continue;
            } else
            {
                std::cerr << "You messed up!" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else if (std::isdigit(c))
        {
            buf.push_back(c);
            i++;
            while (std::isdigit(str.at(i)))
            {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({.type = TokenType::int_lit, .value = buf});
            buf.clear();
        } else if (c == ';')
        {
            tokens.push_back({.type = TokenType::semi});
        } else if (std::isspace(c))
        {
            continue;
        } else
        {
            std::cerr << "You messed up!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
        std::cerr << "tiny <tiny.gcc>" << std::endl;
        return EXIT_FAILURE;
    }

    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();
    
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    std::vector<Token> tokens = tokenize(contents);
    Parser parser (std::move(tokens));
    std::optional<NodeExit> tree = parser.parse();

    if(!tree.has_value()){
        std::cerr < "No exit statement found" << std::endl;
        exit(EXIT_FAILURE);
    }
    Generator generator (tree.value());
    {
        std::fstream("out.asm",std::ios::out);
        file << generator.generate();
    }
    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}