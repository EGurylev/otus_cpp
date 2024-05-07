#include "bulk_server.h"
#include "bulk_router.h"
#include <boost/asio/ip/tcp.hpp>

Session::Session(boost::asio::ip::tcp::socket socket, std::size_t bulk_size)
    : socket_(std::move(socket)), bulk_size_{bulk_size}, router{bulk_size} {}

void Session::start() { do_read(); }

void Session::do_read() {
    auto self(shared_from_this());

    boost::asio::async_read_until(
        socket_, buf_, '\n',
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                do_read();
            }
        });
    std::istream is(&buf_);
    router.process(is);
}

BulkServer::BulkServer(boost::asio::io_service &io_service, uint_least16_t port,
                       std::size_t bulk_size)
    : port_{port}, bulk_size_{bulk_size},
      acceptor_(io_service, boost::asio::ip::tcp::endpoint(
                                boost::asio::ip::tcp::v4(), port_)),
      socket_(io_service) {
    do_accept();
}

void BulkServer::do_accept() {
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
        if (!ec) {
            std::make_shared<Session>(std::move(socket_), bulk_size_)->start();
        }

        do_accept();
    });
}

BulkServer::~BulkServer() { BulkRouter::disconnect_static(); }
