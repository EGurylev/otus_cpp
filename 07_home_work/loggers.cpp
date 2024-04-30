#include "loggers.h"

ConsoleLogger::ConsoleLogger(std::ostream &os) : os_{os} {}

void ConsoleLogger::update(const CommandBlock &cmd_block) { os_ << cmd_block; }

FileLogger::FileLogger(const std::string &name_prefix)
    : name_prefix_{name_prefix} {}

void FileLogger::update(const CommandBlock &cmd_block) {
    const auto first_cmd{*cmd_block.cbegin()};
    std::ofstream ofs(name_prefix_ + std::to_string(first_cmd.time()) + ".log");
    ofs << cmd_block;
}