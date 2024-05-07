#include "bulk_server.h"
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")(
        "port", po::value<uint_least16_t>(), "tcp port number")(
        "bulk_size", po::value<std::size_t>(), "command group size");

    po::positional_options_description pos_desc;
    pos_desc.add("port", 1).add("bulk_size", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(desc)
                  .positional(pos_desc)
                  .run(),
              vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }
    try {
        boost::asio::io_service io_service;
        BulkServer server(io_service, vm["port"].as<uint_least16_t>(),
                          vm["bulk_size"].as<std::size_t>());
        io_service.run();
    } catch (std::exception &exc) {
        std::cout << exc.what() << "\n";
    }
}