#pragma once

#include <string>
#include <vector>
#include <optional>
#include <cctype>
#include <iostream>

enum class TokenType { _return, int_lit, semi };

struct Token {
    TokenType type;
    std::optional<std::string> value{};
};

class Tokenizer {
public:
    explicit Tokenizer(std::string src)
        : m_src(std::move(src)) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string buf;

        while (peek().has_value()) {
            char c = peek().value();

            if (std::isalpha(c)) {
                buf.push_back(consume());
                while (peek().has_value() && std::isalnum(peek().value())) {
                    buf.push_back(consume());
                }

                if (buf == "return") {
                    tokens.push_back({TokenType::_return});
                } else {
                    std::cerr << "Unknown identifier\n";
                    exit(EXIT_FAILURE);
                }
                buf.clear();
            }
            else if (std::isdigit(c)) {
                buf.push_back(consume());
                while (peek().has_value() && std::isdigit(peek().value())) {
                    buf.push_back(consume());
                }
                tokens.push_back({TokenType::int_lit, buf});
                buf.clear();
            }
            else if (c == ';') {
                consume();
                tokens.push_back({TokenType::semi});
            }
            else if (std::isspace(c)) {
                consume();
            }
            else {
                std::cerr << "Invalid character\n";
                exit(EXIT_FAILURE);
            }
        }
        return tokens;
    }

private:
    std::optional<char> peek() const {
        if (m_index >= m_src.size()) return {};
        return m_src[m_index];
    }

    char consume() {
        return m_src[m_index++];
    }

    std::string m_src;
    size_t m_index = 0;
};
