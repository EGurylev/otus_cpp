#include "command_processor.h"

CommandProcessor::CommandProcessor(size_t block_size, std::istream &is,
                                   BlockEdge open, BlockEdge close)
    : block_size_{block_size}, is_{is}, open_{open}, close_{close} {}

void CommandProcessor::subscribe(std::shared_ptr<IObserver> observer) {
    subsribers_.push_back(observer);
}

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
            update();
        }
    }
    update();
}

CommandProcessor::~CommandProcessor() {
    update();
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
        update();
    }
    edges_stack_.push(open_);
}

void CommandProcessor::process_closing() {
    edges_stack_.pop();
    if (edges_stack_.empty()) {
        update();
    }
}

void CommandProcessor::update() {
    if (!cmd_buff_.empty()) {
        for (auto &subscriber : subsribers_) {
            subscriber->update(cmd_buff_);
        }
        cmd_buff_.clear();
    }
}