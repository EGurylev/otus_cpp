#pragma once
#include "command_block.h"
#include <fstream>
#include <iostream>
#include <stack>

class CommandProcessor {
  public:
    struct BlockEdge {
        BlockEdge(char value) : value{value} {}
        char value;
        operator char() const { return value; }
    };

    CommandProcessor(size_t block_size, std::istream &is, std::ostream &os,
                     BlockEdge open = '{', BlockEdge close = '}');

    void process();

  private:
    bool is_opening(const std::string &command) const;

    bool is_closing(const std::string &command) const;

    void process_opening();

    void process_closing();

    void log_to(std::ofstream &&ofs) { ofs << cmd_buff_; }

    void log_to(std::ostream &os) { os << cmd_buff_; }

    void release_commands();

    size_t block_size_{};
    std::istream &is_;
    std::ostream &os_;
    std::stack<BlockEdge> edges_stack_;
    CommandBlock cmd_buff_;
    BlockEdge open_{'{'};
    BlockEdge close_{'}'};
};