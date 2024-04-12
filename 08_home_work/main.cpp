#include "file_group.h"
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace po = boost::program_options;

std::vector<fs::path> get_paths(const std::string &paths) {
    std::vector<fs::path> result;

    std::istringstream ss(paths);
    std::string token;

    while (std::getline(ss, token, ',')) {
        result.push_back(fs::path(token));
    }

    return result;
}

int main(int argc, char *argv[]) {

    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")(
        "include", po::value<std::string>(),
        "directories included to search, coma separated")(
        "exclude", po::value<std::string>()->default_value(""),
        "directories excluded from search, coma separated")(
        "scan_level", po::value<size_t>()->default_value(1),
        "0 - search in top level, 1 - search recursively")(
        "file_mask", po::value<std::string>()->default_value(""),
        "file mask used in a search")("block_size", po::value<uint32_t>(),
                                      "block size in bytes")(
        "checksum", po::value<std::string>()->default_value("sha1"),
        "checksum algorithm: sha1 or crc32")(
        "filter", po::value<std::string>()->default_value(""),
        "regex expression to filter files")(
        "min_size", po::value<std::size_t>()->default_value(1),
        "minimum file size");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }
    try {
        FileGroupManager manager{get_paths(vm["include"].as<std::string>()),
                    get_paths(vm["exclude"].as<std::string>()),
                    static_cast<bool>(vm["scan_level"].as<size_t>()),
                    vm["checksum"].as<std::string>(),
                    vm["block_size"].as<uint32_t>(),
                    vm["filter"].as<std::string>(),
                    vm["min_size"].as<size_t>(), std::cout};

        manager.find_duplicates();

    } catch (std::exception &exc) {
        std::cout << exc.what() << "\n";
    }
}