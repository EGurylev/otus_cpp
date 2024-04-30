#include "async_loggers.h"

namespace async {

AsyncConsoleLogger::AsyncConsoleLogger(std::ostream &os) : os_{os} {
    thread_ = std::jthread(&AsyncConsoleLogger::process, this);
}

void AsyncConsoleLogger::update(const CommandBlock &cmd_block) {
    std::unique_lock lock(m_);
    command_blocks_.push(cmd_block);
    lock.unlock();
    cv_.notify_one();
}

void AsyncConsoleLogger::process() {
    while (true) {
        std::unique_lock lock(m_);
        cv_.wait(lock, [this]() { return !command_blocks_.empty() || stop_; });
        if (stop_ && command_blocks_.empty()) {
            break;
        }
        os_ << command_blocks_.front();
        command_blocks_.pop();
    }
}

void AsyncConsoleLogger::stop() {
    stop_ = true;
    cv_.notify_one();
}

AsyncDuoFileLogger::AsyncDuoFileLogger(const std::string &name_prefix)
    : name_prefix_{name_prefix} {
    first_thread_ = std::jthread(&AsyncDuoFileLogger::write, this);
    second_thread_ = std::jthread(&AsyncDuoFileLogger::write, this);
}

void AsyncDuoFileLogger::update(const CommandBlock &cmd_block) {
    std::unique_lock lock(m_);
    command_blocks_.push(cmd_block);
    lock.unlock();
    cv_.notify_all();
}

void AsyncDuoFileLogger::write() {
    while (true) {
        std::unique_lock lock(m_);
        cv_.wait(lock, [this]() { return !command_blocks_.empty() || stop_; });
        if (stop_ && command_blocks_.empty()) {
            break;
        }
        auto cmd_block{command_blocks_.front()};
        command_blocks_.pop();
        ++block_counter_;
        lock.unlock();
        const auto first_cmd{*cmd_block.cbegin()};
        std::ofstream ofs(name_prefix_ + std::to_string(block_counter_) + "_" +
                          std::to_string(first_cmd.time()) + ".log");
        ofs << cmd_block;
    }
}

void AsyncDuoFileLogger::stop() {
    stop_ = true;
    cv_.notify_all();
}

} // namespace async