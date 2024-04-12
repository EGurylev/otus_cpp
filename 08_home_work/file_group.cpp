#include "file_group.h"
#include <unordered_set>
#ifdef __linux__
#include <sys/resource.h>
#endif

std::ostream &
operator<<(std::ostream &os,
           std::unordered_map<size_t, FileGroupManager::FileGroup> &groups) {
    for (auto it{groups.begin()}; it != groups.end(); ++it) {
        if (it != groups.begin()) {
            os << std::endl;
        }
        for (const auto &duplicate : it->second.get_files()) {
            os << duplicate.first.c_str() << std::endl;
        }
    }
    return os;
}

FileGroupManager::FileGroupManager(std::vector<fs::path> &&include,
                                   std::vector<fs::path> &&exclude,
                                   bool recursive, const std::string &checksum,
                                   uint32_t block_size,
                                   const std::string &filter, size_t min_size,
                                   std::ostream &os)
    : include_{std::move(include)}, exclude_{exclude.begin(), exclude.end()},
      recursive_{recursive}, filter_{filter, boost::regex::icase},
      min_size_{min_size}, os_{os} {
    if (checksum == "crc32") {
        FileHandler::set_checksum(Checksum::crc32);
    } else if (checksum == "sha1") {
        FileHandler::set_checksum(Checksum::sha1);
    } else {
        std::cout << "Unrecognized checksum option. sha1 will be used as a "
                     "default.\n";
    }
    FileHandler::set_block_size(block_size);
}

void FileGroupManager::find_groups() {
    const auto size_groups{
        !recursive_ ? group_by_size<fs::directory_iterator>()
                    : group_by_size<fs::recursive_directory_iterator>()};
    for (auto &size_group : size_groups) {
        FileGroup group;
        for (auto &file : size_group.second) {
            group[file] = FileHandler(file);
        }
        file_groups_[group_id_++] = std::move(group);
    }
}

bool FileGroupManager::all_finished() const {
    for (auto &group : file_groups_) {
        if (!group.second.is_finished()) {
            return false;
        }
    }
    return true;
}

size_t FileGroupManager::get_file_limit() {
    size_t limit{std::numeric_limits<size_t>::max()};
#ifdef __linux__
    struct rlimit lim;
    getrlimit(RLIMIT_NOFILE, &lim);
    limit = lim.rlim_cur;
#endif
    return limit;
}

void FileGroupManager::find_duplicates() {
    find_groups();

    if (count_files() >= get_file_limit()) {
        throw std::runtime_error("Program cannot exceed limit to open files " +
                                 std::to_string(get_file_limit()) +
                                 ". Actual number is " +
                                 std::to_string(count_files()));
    }

    while (!all_finished()) {
        make_step();
    }
}

void FileGroupManager::print_duplicates() { os_ << file_groups_; }

size_t FileGroupManager::count_files() {
    size_t num_files_to_open{};
    for (auto &group : file_groups_) {
        num_files_to_open += group.second.get_files().size();
    }
    return num_files_to_open;
}

void FileGroupManager::make_step() {
    std::unordered_set<size_t> groups_to_remove;
    for (auto &group : file_groups_) {
        if (group.second.is_finished()) {
            continue;
        }
        std::unordered_map<std::string, std::vector<fs::path>> hash_groups;
        for (auto &file : group.second.get_files()) {
            hash_groups[file.second.get_next_hash()].push_back(file.first);
        }

        // Split group
        if (hash_groups.size() > 1) {
            groups_to_remove.insert(group.first);
            for (auto &hash_group : hash_groups) {
                if (hash_group.second.size() > 1) {
                    FileGroup new_group;
                    for (auto &file : hash_group.second) {
                        new_group[file] = std::move(group.second[file]);
                    }
                    file_groups_[group_id_++] = std::move(new_group);
                }
            }
        }
    }
    std::erase_if(file_groups_, [&groups_to_remove](const auto &item) {
        return groups_to_remove.find(item.first) != groups_to_remove.end();
    });
}

size_t FileGroupManager::group_id_{};