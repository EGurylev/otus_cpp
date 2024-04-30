#include "async_loggers.h"

namespace async {

AsyncConsoleLogger::AsyncConsoleLogger(std::ostream &os) : os_{os} {
    thread_ = std::thread(&AsyncConsoleLogger::process, this);
}

void AsyncConsoleLogger::update(const CommandBlock &cmd_block) {
    std::unique_lock lock(m_);
    cv_.wait(lock, [this]() { return cmd_block_.empty(); });
    cmd_block_ = cmd_block;
    cv_.notify_one();
}

AsyncConsoleLogger::~AsyncConsoleLogger() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

void AsyncConsoleLogger::process() {
    while (true) {
        std::unique_lock lock(m_);
        cv_.wait(lock, [this]() { return !cmd_block_.empty() || stop_; });
        if (stop_) {
            break;
        }
        os_ << cmd_block_;
        cmd_block_.clear();
        cv_.notify_one();
    }
}

void AsyncConsoleLogger::stop() {
    stop_ = true;
    cv_.notify_one();
}

} // namespace async