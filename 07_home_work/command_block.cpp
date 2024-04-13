#include "command_block.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const CommandBlock &cmd_block) {
    if (!cmd_block.empty()) {
        os << "bulk: ";
    }
    for (auto it{cmd_block.cbegin()}; it != cmd_block.cend(); ++it) {
        if (it != cmd_block.cbegin()) {
            os << ", ";
        }
        os << it->body();
    }
    if (!cmd_block.empty()) {
        os << std::endl;
    }
    return os;
}