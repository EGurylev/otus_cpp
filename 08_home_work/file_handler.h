#pragma once
#include <boost/crc.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

namespace fs = boost::filesystem;

struct HashPath {
    size_t operator()(const fs::path &path) const {
        return std::hash<std::string>{}(path.string());
    }
};

enum class Checksum { crc32, sha1 };

class FileHandler {
  public:
    FileHandler() = default;
    FileHandler(const fs::path &path);

    std::string get_next_hash() const;

    size_t size() const { return size_; }

    bool is_finished() const;

    static void set_block_size(size_t block_size);
    static void set_checksum(Checksum checksum) { checksum_ = checksum; }

  private:
    std::string sha1(const std::string &str) const;
    std::string crc32(const std::string &str) const;

    fs::path path_;
    mutable std::ifstream ifs_;
    static Checksum checksum_;
    static size_t block_size_;
    size_t size_;
    mutable size_t pos_{};
    mutable std::string hash_;
};