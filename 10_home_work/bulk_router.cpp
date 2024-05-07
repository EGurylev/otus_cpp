#include "bulk_router.h"
#include "async.h"

BulkRouter::BulkRouter(std::size_t bulk_size) : bulk_size_{bulk_size} {
    if (!static_block_connection_) {
        static_block_connection_ = async::connect(bulk_size_);
    }
}

BulkRouter::~BulkRouter() {
    if (dynamic_block_connection_) {
        async::disconnect(dynamic_block_connection_);
    }
}

void BulkRouter::disconnect_static() {
    if (static_block_connection_) {
        async::disconnect(static_block_connection_);
    }
}

void *BulkRouter::static_block_connection_{};
std::string BulkRouter::static_block_;
std::size_t BulkRouter::curr_static_block_size_{};

void BulkRouter::process(std::istream &is) {
    std::string line;
    if (std::getline(is, line)) {
        if (line == "{") {
            braces_.push(line.front());
            dynamic_block_ += line + "\n";
        } else if (line == "}" && !braces_.empty() && braces_.top() == '{') {
            braces_.pop();
            dynamic_block_ += line + "\n";
            if (braces_.empty() || is.peek() == -1) {
                if (!dynamic_block_connection_) {
                    dynamic_block_connection_ = async::connect(bulk_size_);
                }
                async::receive(dynamic_block_connection_,
                               dynamic_block_.c_str(), dynamic_block_.size());
            }
        } else if (!braces_.empty()) {
            dynamic_block_ += line + "\n";
        } else {
            static_block_ += line + "\n";
            ++curr_static_block_size_;
            if (curr_static_block_size_ == bulk_size_ || is.peek() == -1) {
                async::receive(static_block_connection_, static_block_.c_str(),
                               static_block_.size());
                curr_static_block_size_ = 0;
                static_block_.clear();
            }
        }
    }
}