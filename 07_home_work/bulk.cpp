#include "command_processor.h"

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        const auto command_block_size{std::stoi(argv[1])};
        if (command_block_size < 0) {
            std::cout << "Block size cannot be negative.\n";
            return 0;
        }
        CommandProcessor cmd_proc(command_block_size, std::cin, std::cout);
        cmd_proc.process();
    }
    else {
        std::cout << "You have to enter block size.\n";
    }
}