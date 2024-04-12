#include "file_handler.h"
#include <boost/crc.hpp>
#include <boost/uuid/detail/sha1.hpp>

FileHandler::FileHandler(const fs::path &path) : path_{path} {
    size_ = fs::file_size(path_);
}

void FileHandler::set_block_size(size_t block_size) {
    if (block_size >= 1) {
        block_size_ = block_size;
    } else {
        throw std::runtime_error(
            "Block size must be greater or equal to 1 byte");
    }
}

size_t FileHandler::block_size_{1};
Checksum FileHandler::checksum_{Checksum::sha1};

bool FileHandler::is_finished() const { return pos_ >= size_; }

std::string FileHandler::get_next_hash() const {
    if (pos_ > size_) {
        return hash_;
    }
    if (!ifs_.is_open()) {
        ifs_.open(path_, std::ios::in | std::ios::binary);
    }

    std::string chunk(block_size_, 0);
    ifs_.read(chunk.data(), block_size_);
    pos_ += block_size_;

    hash_ = (checksum_ == Checksum::sha1) ? sha1(chunk) : crc32(chunk);

    return hash_;
}

std::string FileHandler::sha1(const std::string &str) const {
    boost::uuids::detail::sha1 sha;
    sha.process_bytes(str.data(), str.size());
    unsigned hash[5] = {0};
    sha.get_digest(hash);

    // back to string
    char buf[41] = {0};
    for (int i = 0; i < 5; ++i) {
        std::sprintf(buf + (i << 3), "%08x", hash[i]);
    }

    return buf;
}

std::string FileHandler::crc32(const std::string &str) const {
    boost::crc_32_type result;
    result.process_bytes(str.data(), str.length());
    return std::to_string(result.checksum());
}