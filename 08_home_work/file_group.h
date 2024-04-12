#pragma once
#include "file_handler.h"
#include <boost/range/adaptors.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <unordered_set>

class FileGroupManager {
  public:
    class FileGroup {
      public:
        FileHandler &operator[](const fs::path &path) { return group_[path]; }

        std::unordered_map<fs::path, FileHandler, HashPath> &get_files() {
            return group_;
        }

        bool is_finished() const { return group_.begin()->second.is_finished(); }

      private:
        std::unordered_map<fs::path, FileHandler, HashPath> group_;
    };

    FileGroupManager(std::vector<fs::path> &&include,
                     std::vector<fs::path> &&exclude, bool recursive,
                     const std::string &checksum, uint32_t block_size,
                     const std::string &filter, size_t min_size,
                     std::ostream &os);

    bool all_finished() const;

    void make_step();
    void find_duplicates();

  private:
    void find_groups();
    size_t count_files();
    size_t get_file_limit();

    template <typename DirectoryIterator> auto group_by_size() {
        std::unordered_map<size_t, std::vector<fs::path>> size_groups;
        for (const auto &directory : include_) {
            boost::smatch what;

            DirectoryIterator it{directory}, end;
            while (it != end) {
                if constexpr (std::is_same_v<
                                  DirectoryIterator,
                                  fs::recursive_directory_iterator>) {
                    if (exclude_.find(it->path()) != exclude_.end()) {
                        it.no_push();
                    }
                }

                if (fs::is_regular_file(it->path()) &&
                    fs::file_size(it->path()) >= min_size_ &&
                    (filter_.str().empty()
                         ? true
                         : boost::regex_match(it->path().filename().string(),
                                              what, filter_))) {
                    size_groups[fs::file_size(it->path())].push_back(
                        it->path());
                }
                ++it;
            }

            std::erase_if(size_groups, [](const auto &item) {
                return item.second.size() == 1;
            });
        }
        return size_groups;
    }

    std::vector<fs::path> include_;
    std::unordered_set<fs::path, HashPath> exclude_;
    bool recursive_{};
    uint32_t block_size_;
    boost::regex filter_;
    size_t min_size_;
    std::ostream &os_;

    std::unordered_map<size_t, FileGroup> file_groups_;
    static size_t group_id_;
};

std::ostream &operator<<(
    std::ostream &os,
    const std::unordered_map<size_t, FileGroupManager::FileGroup> &groups);