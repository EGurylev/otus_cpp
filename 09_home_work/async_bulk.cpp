#include "async_bulk.h"
#include <iostream>

namespace async {

Connection::Connection(std::size_t bulk) : cmd_proc_(bulk, ss_) {}

void Connection::subscribe(std::shared_ptr<IObserver> observer) {
    cmd_proc_.subscribe(observer);
}

void Connection::update(const char *data, std::size_t size) {
    ss_.clear();
    ss_.write(data, size);
    cmd_proc_.process();
}

AsyncBulk::AsyncBulk()
    : console_logger_{std::make_shared<AsyncConsoleLogger>(std::cout)},
      files_logger_{std::make_shared<AsyncDuoFileLogger>("bulk")} {}

AsyncBulk::~AsyncBulk() {
    console_logger_->stop();
    files_logger_->stop();
}

AsyncBulk &AsyncBulk::getInstance() {
    static AsyncBulk instance;
    return instance;
}

void AsyncBulk::update(Connection *connection, const char *data, std::size_t size) {
    std::unique_lock lock(connection_mtx_);
    connections_[connection]->update(data, size);
}

void AsyncBulk::remove_connection(Connection *connection) {
    std::unique_lock lock(connection_mtx_);
    connections_.erase(connection);
}

Connection *AsyncBulk::create_connection(std::size_t bulk) {
    auto connection{std::make_unique<Connection>(bulk)};
    auto conn_ptr{connection.get()};
    connection->subscribe(console_logger_);
    connection->subscribe(files_logger_);

    std::unique_lock lock(connection_mtx_);
    connections_[conn_ptr] = std::move(connection);

    return conn_ptr;
}

} // namespace async