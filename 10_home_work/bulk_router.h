#pragma once
#include "stack"
#include <istream>

class BulkRouter {
  public:
    BulkRouter() = default;
    BulkRouter(std::size_t bulk_size);

    ~BulkRouter();

    static void disconnect_static();

    void process(std::istream &is);

  private:
    static void *static_block_connection_;
    static std::string static_block_;
    static std::size_t curr_static_block_size_;

    std::stack<char> braces_;
    std::string dynamic_block_;
    void *dynamic_block_connection_{};
    std::size_t bulk_size_;
};