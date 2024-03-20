#include "command_processor.h"

CommandProcessor::CommandProcessor(size_t block_size, std::istream &is,
                                   std::ostream &os, BlockEdge open,
                                   BlockEdge close)
    : block_size_{block_size}, is_{is}, os_{os}, open_{open}, close_{close} {}

void CommandProcessor::process() {

    std::string command;
    while (std::getline(is_, command)) {
        if (is_opening(command)) {
            process_opening();
        } else if (is_closing(command)) {
            process_closing();
        } else {
            cmd_buff_.push(command);
        }

        if (cmd_buff_.size() == block_size_ && edges_stack_.empty()) {
            release_commands();
        }
    }
    release_commands();
}

bool CommandProcessor::is_opening(const std::string &command) const {
    return command.size() == 1 && command.front() == open_;
}

bool CommandProcessor::is_closing(const std::string &command) const {
    return command.size() == 1 && command.front() == close_ &&
           !edges_stack_.empty() && edges_stack_.top() == open_;
}

void CommandProcessor::process_opening() {
    if (edges_stack_.empty()) {
        release_commands();
    }
    edges_stack_.push(open_);
}

void CommandProcessor::process_closing() {
    edges_stack_.pop();
    if (edges_stack_.empty()) {
        release_commands();
    }
}

void CommandProcessor::release_commands() {
    if (!cmd_buff_.empty()) {
        const auto first_cmd{*cmd_buff_.cbegin()};
        log_to(os_);
        log_to(
            std::ofstream("bulk" + std::to_string(first_cmd.time()) + ".log"));
        cmd_buff_.clear();
    }
}

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