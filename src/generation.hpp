#pragma once

#include <sstream>
#include <string>
#include "parser.hpp"

class Generator {
public:
    explicit Generator(const NodeExit& root)
        : m_root(root) {}

    std::string generate() const {
        std::stringstream output;

        output << "global _start\n";
        output << "_start:\n";
        output << "    mov rax, 60\n";
        output << "    mov rdi, " << m_root.expr.int_lit.value.value() << "\n";
        output << "    syscall\n";

        return output.str();
    }

private:
    const NodeExit& m_root;
};
