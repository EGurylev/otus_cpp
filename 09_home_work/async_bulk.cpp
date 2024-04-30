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
    : console_logger_{std::make_shared<AsyncConsoleLogger>(std::cout)} {}

AsyncBulk::~AsyncBulk() { console_logger_->stop(); }

AsyncBulk &AsyncBulk::getInstance() {
    static AsyncBulk instance;
    return instance;
}

void AsyncBulk::update(Connection *conn_ptr, const char *data, size_t size) {
    std::unique_lock lock(connection_mtx_);
    connections_[conn_ptr]->update(data, size);
}

void AsyncBulk::remove_connection(Connection *conn_ptr) {
    std::unique_lock lock(connection_mtx_);
    connections_.erase(conn_ptr);
}

Connection *AsyncBulk::create_connection(std::size_t bulk) {
    std::unique_lock lock(connection_mtx_);
    auto conn{std::make_unique<Connection>(bulk)};
    auto conn_ptr{conn.get()};
    conn->subscribe(console_logger_);
    connections_[conn_ptr] = std::move(conn);

    return conn_ptr;
}

} // namespace async