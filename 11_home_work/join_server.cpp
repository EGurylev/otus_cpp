#include "join_server.h"
#include <boost/asio/ip/tcp.hpp>

Session::Session(boost::asio::ip::tcp::socket socket,
                 database::CommandProcessor &command_processor)
    : socket_(std::move(socket)), command_processor_{command_processor} {}

void Session::start() { do_read(); }

void Session::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(
        boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                auto response{command_processor_.process(data_)};
                std::fill(data_, data_ + max_length, 0);
                std::copy(response.begin(), response.end(), data_);
                do_write(response.size());
            }
        });
}

void Session::do_write(std::size_t length) {
    auto self(shared_from_this());
    boost::asio::async_write(
        socket_, boost::asio::buffer(data_, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                std::fill(data_, data_ + max_length, 0);
                do_read();
            }
        });
}

JoinServer::JoinServer(boost::asio::io_service &io_service, uint_least16_t port)
    : port_{port},
      acceptor_(io_service, boost::asio::ip::tcp::endpoint(
                                boost::asio::ip::tcp::v4(), port_)),
      socket_(io_service) {
    do_accept();
}

void JoinServer::do_accept() {
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
        if (!ec) {
            std::make_shared<Session>(std::move(socket_), command_processor_)
                ->start();
        }

        do_accept();
    });
}
