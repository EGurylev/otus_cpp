#pragma once
#include "async.h"
#include "bulk_router.h"
#include <boost/asio.hpp>

#include <iostream>

class Session : public std::enable_shared_from_this<Session> {
  public:
    Session(boost::asio::ip::tcp::socket socket, std::size_t bulk_size);

    void start();

  private:
    void do_read();

    boost::asio::ip::tcp::socket socket_;
    std::size_t bulk_size_;
    BulkRouter router;
    boost::asio::streambuf buf_;
};

class BulkServer {
  public:
    BulkServer(boost::asio::io_service &io_service, uint_least16_t port,
               std::size_t bulk_size);

    ~BulkServer();

  private:
    void do_accept();

    uint_least16_t port_{};
    std::size_t bulk_size_{};
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};