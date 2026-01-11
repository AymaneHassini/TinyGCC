#pragma once

#include "tokenization.hpp"
#include <optional>
#include <iostream>

struct NodeExpr {
    Token int_lit;
};

struct NodeExit {
    NodeExpr expr;
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens)) {}

    std::optional<NodeExpr> parse_expr() {
        if (peek().has_value() && peek()->type == TokenType::int_lit) {
            return NodeExpr{ consume() };
        }
        return {};
    }

    std::optional<NodeExit> parse() {
        std::optional<NodeExit> exit_node;

        while (peek().has_value()) {
            if (peek()->type == TokenType::_return) {
                consume();

                auto expr = parse_expr();
                if (!expr) {
                    std::cerr << "Expected expression after return\n";
                    exit(EXIT_FAILURE);
                }

                exit_node = NodeExit{ expr.value() };

                if (!peek().has_value() || peek()->type != TokenType::semi) {
                    std::cerr << "Missing semicolon\n";
                    exit(EXIT_FAILURE);
                }
                consume();
            } else {
                std::cerr << "Unexpected token\n";
                exit(EXIT_FAILURE);
            }
        }

        return exit_node;
    }

private:
    std::optional<Token> peek() const {
        if (m_index >= m_tokens.size()) return {};
        return m_tokens[m_index];
    }

    Token consume() {
        return m_tokens[m_index++];
    }

    std::vector<Token> m_tokens;
    size_t m_index = 0;
};
