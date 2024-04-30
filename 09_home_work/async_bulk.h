#pragma once
#include "async_loggers.h"
#include "command_processor.h"
#include <mutex>
#include <sstream>
#include <unordered_map>

namespace async {

class Connection {
  public:
    Connection(std::size_t bulk);

    void subscribe(std::shared_ptr<IObserver> observer);

    void update(const char *data, std::size_t size);

  private:
    std::stringstream ss_;
    CommandProcessor cmd_proc_;
};

class AsyncBulk {
  public:
    static AsyncBulk &getInstance();

    Connection *create_connection(std::size_t bulk);

    void update(Connection *connection, const char *data, std::size_t size);

    void remove_connection(Connection *conn_ptr);

    AsyncBulk();

    ~AsyncBulk();

    AsyncBulk(const AsyncBulk &) = delete;
    void operator=(const AsyncBulk &) = delete;

  private:
    std::unordered_map<Connection *, std::unique_ptr<Connection>> connections_;
    std::shared_ptr<AsyncConsoleLogger> console_logger_;
    std::shared_ptr<AsyncDuoFileLogger> files_logger_;
    std::mutex connection_mtx_;
};

} // namespace async