#pragma once
#include "database_command_processor.h"
#include <boost/asio.hpp>

#include <iostream>

class Session : public std::enable_shared_from_this<Session> {
  public:
    Session(boost::asio::ip::tcp::socket socket,
            database::CommandProcessor &command_processor);

    void start();

  private:
    void do_read();
    void do_write(std::size_t length);

    boost::asio::ip::tcp::socket socket_;
    std::size_t bulk_size_;
    enum { max_length = 1024 };
    char data_[max_length];
    database::CommandProcessor &command_processor_;
};

class JoinServer {
  public:
    JoinServer(boost::asio::io_service &io_service, uint_least16_t port);

  private:
    void do_accept();

    uint_least16_t port_{};
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
    database::CommandProcessor command_processor_;
};